// Fill out your copyright notice in the Description page of Project Settings.


#include "AIBot.h"

#include "AITracker.h"
#include "NavigationSystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"

void AAIBot::OnPossess(APawn* InPawn) //When AI Enters Pawn Body 
{
	Super::OnPossess(InPawn);//Gets Self and the AI Path
	Self = Cast<ATarget>(GetPawn()); 
	AiPath = Cast<AAIPath>(UGameplayStatics::GetActorOfClass(GetWorld(), AAIPath::StaticClass()));

	FTimerHandle TimerHandle; //Adds Bind to the weapon that will work out recoil when the shooting starts
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]() 
	{
		Self->AIRifle->RecoilDiv.AddDynamic(this, &AAIBot::RecoilDivision);
		Self->AIRifle->BulletMissed.AddDynamic(this, &AAIBot::BulletMissedResetAim);
		}, 1.0f, false);

	FollowAIPath();


	UAITracker* MyComponent = Cast<UAITracker>(Self->GetComponentByClass(UAITracker::StaticClass()));
	if(MyComponent)
	{
		MyComponent->StartTracking(this, Self, Self->AIRifle);
	}
}

void AAIBot::BeginPlay() //Starts the walking Process
{
	Super::BeginPlay();
	
	
}

void AAIBot::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) //When Reaches Waypoint or when stopped
{
	
	if(!bIntentionallyStoppedMovement) //When reached way point -> To the next one
	{
		CurrentPathIndex++;
		FollowAIPath();
		Super::OnMoveCompleted(RequestID, Result);
	}
	else //Will not run next waypoint if stop Intentionally 
	{
		bIntentionallyStoppedMovement = false;
	}
	
	
}

AAIBot::AAIBot(FObjectInitializer const& ObjectInitializer) //Sets up sight on construct
{
	SetupSight();
}

void AAIBot::SetupSight()
{
	SenseSight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sense"));
	if(SenseSight)
	{
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception")));
		SenseSight->SightRadius = SightRadius;
		SenseSight->LoseSightRadius = LoseRadius;
		SenseSight->SetMaxAge(5.0f);
		SenseSight->AutoSuccessRangeFromLastSeenLocation = 100.0f;
		SenseSight->DetectionByAffiliation.bDetectNeutrals = true;
		SenseSight->DetectionByAffiliation.bDetectFriendlies  = true;
		SenseSight->DetectionByAffiliation.bDetectEnemies = true;
		GetPerceptionComponent()->SetDominantSense(*SenseSight->GetSenseImplementation());
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AAIBot::TargetUpdate); //Bind to Function when object enters sight or leaves
		GetPerceptionComponent()->ConfigureSense(*SenseSight);
		
	}
}

void AAIBot::StopMovementAfterDelay() //Delay to simulate Human Reaction times of seeing to stopping -> Inaccurary when moving and shooting
{
	bIntentionallyStoppedMovement = true; 
	StopMovement();
}

void AAIBot::StartFiringAfterDelay() //Delay to simulate Human Reaction Times of seeing to shooting
{
	SetFocus(CurrentlySensedActors[0]);
	Self->AIRifle->ToggleFire(true, CalculateAimTarget());
}

void AAIBot::RecoilDivision(int CurrentSprayBullet) //Calculation of the recoil using the AimCurve the bigger the more accurate
{
	float AimDivider = AimCurve->GetFloatValue(CurrentSprayBullet) + FMath::RandRange(0.0f, VarietyCurve->GetFloatValue(CurrentSprayBullet));
	Self->AIRifle->RecoilDivider = AimDivider;
}

FVector AAIBot::CalculateAimTarget()
{
	int RandomNumber = FMath::RandRange(1, 100);
	if (RandomNumber <= HeadshotPercentageAim)
	{
		return Cast<ATarget>(CurrentlySensedActors[0])->GetHeadLocation();
		
	}
	return Cast<ATarget>(CurrentlySensedActors[0])->GetBodyLocation(); 
}

void AAIBot::BulletMissedResetAim()
{
	BulletMissCount++;
	if(BulletMissCount >= BulletMissedResetAmount)
	{
		Self->AIRifle->ToggleFire(false, FVector::ZeroVector);
		FTimerHandle ResetAimTime;
		GetWorld()->GetTimerManager().SetTimer(ResetAimTime, this, &AAIBot::StartFiringAfterDelay , TimeForAimToResetAfterMissed, false);
		BulletMissCount = 0;
	}
}

void AAIBot::InitializeController(FString Name, float HeadShotPercentage, int BulletMissResetCount, float AimResetSpeed,
	UCurveFloat* AimCurves, UCurveFloat* VarCurve, float FiringReactionLower, float FiringReactionUpper, float WalkingReactionLower,
	float WalkingReactionUpper)
{
	BotName = Name;
	HeadshotPercentageAim = HeadShotPercentage;
	BulletMissedResetAmount = BulletMissResetCount;
	TimeForAimToResetAfterMissed = AimResetSpeed;
	AimCurve = AimCurves;
	FiringReactionLowerBound = FiringReactionLower;
	FiringReactionUpperBound = FiringReactionUpper;
	WalkingReactionLowerBound = WalkingReactionLower;
	WalkingReactionUpperBound = WalkingReactionUpper;
	VarietyCurve = VarCurve;
}

void AAIBot::TargetUpdate(AActor* SeenActor, FAIStimulus const Stim)
{
	if(Cast<ATarget>(SeenActor) && Stim.WasSuccessfullySensed()) //If has seen ATarget Enter Sight Sense
	{
		CurrentlySensedActors.Add(SeenActor);
		
		//------------ Debug Lines -----------------// Also Sets Head Rotation Maybe Not Needed?
		FVector HeadLocation = Self->HeadDirectionsComp->GetComponentLocation();
		FVector TargetLocation = SeenActor->GetActorLocation();
		FVector Direction = (TargetLocation - HeadLocation).GetSafeNormal();
		FRotator NewRotation = Direction.Rotation();
		Self->HeadDirectionsComp->SetWorldRotation(NewRotation);
		FVector Start = HeadLocation;
		FVector End = Start + Self->HeadDirectionsComp->GetForwardVector() * 10000; 
		FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel(HitResult, Start, End,ECC_Visibility, FCollisionQueryParams(FName(TEXT("")), false, this) );
		DrawDebugLine(GetWorld(), Start, End, FColor::Green, false,  5.0f,   0,      2.0f    );
		//------------ Debug Lines -----------------//
		SeenEnemy.Broadcast();

		
		//Binds to Death so can carry on once killed Target
		Cast<ATarget>(SeenActor)->OnDeath.AddDynamic(this, &AAIBot::KilledTarget);

		//Stops the AI Facing the Direction of movement to look at the enemy
		Self->GetCharacterMovement()->bOrientRotationToMovement = false;

		//Aiming at the Actor when Seen Target
		float RandomFiringReactionTime = FMath::RandRange(FiringReactionLowerBound, FiringReactionUpperBound);
		GetWorldTimerManager().SetTimer(FiringReactionTimerHandle, this, &AAIBot::StartFiringAfterDelay, RandomFiringReactionTime, false);

		//Stopping The AI when Seen Target
		float RandomWalkingReactionTime = FMath::RandRange(WalkingReactionLowerBound, WalkingReactionUpperBound);
		GetWorldTimerManager().SetTimer(WalkingReactionTimerHandle, this, &AAIBot::StopMovementAfterDelay, RandomWalkingReactionTime, false);

	}
	else //When Lost Target - Stop Firing and Clears Focus so looks the way of movement - Continues Path
	{
		
		Self->AIRifle->ToggleFire(false, FVector::ZeroVector);
		ClearFocus(EAIFocusPriority::Gameplay);
		ContinuePath();
	}
}

void AAIBot::FollowAIPath()
{
	Self->GetCharacterMovement()->bOrientRotationToMovement = true; //Sets Look at walking Direction
	if(CurrentPathIndex < AiPath->Num()) //Path Avaiblible 
	{
		MoveToLocation(AiPath->GetPatrolPoint(CurrentPathIndex));
	}
}

void AAIBot::ContinuePath()
{
	Self->GetCharacterMovement()->bOrientRotationToMovement = true;
	FollowAIPath();
	bIntentionallyStoppedMovement = false;
}

void AAIBot::KilledTarget() //Callback to when killed a Target - Stops Firing and Starts Movement Again
{
	KilledEnemy.Broadcast();
	CurrentlySensedActors.RemoveAt(0);
	Self->AIRifle->ToggleFire(false, FVector::ZeroVector);
	if(CurrentlySensedActors.IsEmpty())
	{
		ContinuePath();
	}
	else
	{
		StartFiringAfterDelay();
	}
	
}

