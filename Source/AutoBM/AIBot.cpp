// Fill out your copyright notice in the Description page of Project Settings.


#include "AIBot.h"

#include "NavigationSystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"

void AAIBot::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	Self = Cast<ATarget>(GetPawn());
	AiPath = Cast<AAIPath>(UGameplayStatics::GetActorOfClass(GetWorld(), AAIPath::StaticClass()));
	
}

void AAIBot::BeginPlay()
{
	Super::BeginPlay();
	FollowAIPath();
	
}

void AAIBot::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if(!bIntentionallyStoppedMovement)
	{
		UE_LOG(LogTemp,Warning,TEXT("Has Ended Movement"));
		CurrentPathIndex++;
		FollowAIPath();
		Super::OnMoveCompleted(RequestID, Result);
	}
	else
	{
		bIntentionallyStoppedMovement = false;
	}
	
	
}

AAIBot::AAIBot(FObjectInitializer const& ObjectInitializer)
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
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AAIBot::TargetUpdate);
		UE_LOG(LogTemp, Warning, TEXT("Add Dynamic"));
		GetPerceptionComponent()->ConfigureSense(*SenseSight);
		
	}
}

void AAIBot::StopMovementAfterDelay()
{
	bIntentionallyStoppedMovement = true;
	StopMovement();
}

void AAIBot::StartFiringAfterDelay()
{
	SetFocus(TempActor);
	Self->AIRifle->ToggleFire(true, TempActor);
}

void AAIBot::TargetUpdate(AActor* SeenActor, FAIStimulus const Stim)
{
	UE_LOG(LogTemp, Warning, TEXT("Updated Perception"));
	if(Cast<ATarget>(SeenActor) && Stim.WasSuccessfullySensed())
	{
		
		FVector HeadLocation = Self->HeadDirectionsComp->GetComponentLocation();
		FVector TargetLocation = SeenActor->GetActorLocation();
		FVector Direction = (TargetLocation - HeadLocation).GetSafeNormal();
		FRotator NewRotation = Direction.Rotation();
		Self->HeadDirectionsComp->SetWorldRotation(NewRotation);
		FVector Start = HeadLocation;
		FVector End = Start + Self->HeadDirectionsComp->GetForwardVector() * 10000; 
		FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel(HitResult, Start,     End,      ECC_Visibility, FCollisionQueryParams(FName(TEXT("")), false, this) );
		DrawDebugLine(GetWorld(), Start, End, FColor::Green, false,  5.0f,   0,      2.0f    );

		//Binds to Death so can carry on once killed Target
		Cast<ATarget>(SeenActor)->OnDeath.AddDynamic(this, &AAIBot::KilledTarget);

		//Stops the AI Facing the Direction of movement to look at the enemy
		Self->GetCharacterMovement()->bOrientRotationToMovement = false;

		//Aiming at the Actor when Seen Target
		TempActor = SeenActor;
		float RandomFiringReactionTime = FMath::RandRange(FiringReactionLowerBound, FiringReactionUpperBound);
		GetWorldTimerManager().SetTimer(FiringReactionTimerHandle, this, &AAIBot::StartFiringAfterDelay, RandomFiringReactionTime, false);

		//Stopping The AI when Seen Target
		float RandomWalkingReactionTime = FMath::RandRange(WalkingReactionLowerBound, WalkingReactionUpperBound);
		GetWorldTimerManager().SetTimer(WalkingReactionTimerHandle, this, &AAIBot::StopMovementAfterDelay, RandomWalkingReactionTime, false);
		
		UE_LOG(LogTemp, Warning, TEXT("Random Firing Reaction Time: %f seconds"), RandomFiringReactionTime);
		UE_LOG(LogTemp, Warning, TEXT("Random Walking Reaction Time: %f seconds"), RandomWalkingReactionTime);
	}
	else
	{
		
		UE_LOG(LogTemp, Warning, TEXT("No Target"));
		Self->AIRifle->ToggleFire(false, nullptr);
		ClearFocus(EAIFocusPriority::Gameplay);
		ContinuePath();
	}
}

void AAIBot::FollowAIPath()
{
	Self->GetCharacterMovement()->bOrientRotationToMovement = true;
	if(CurrentPathIndex < AiPath->Num())
	{
		MoveToLocation(AiPath->GetPatrolPoint(CurrentPathIndex));
		UE_LOG(LogTemp, Warning, TEXT("Has Moved"));
	}
}

void AAIBot::ContinuePath()
{
	Self->GetCharacterMovement()->bOrientRotationToMovement = true;
	FollowAIPath();
	bIntentionallyStoppedMovement = false;
}

void AAIBot::KilledTarget()
{
	UE_LOG(LogTemp, Warning, TEXT("Killed Enemy"));
	Self->AIRifle->ToggleFire(false, nullptr);
	ContinuePath();
}








