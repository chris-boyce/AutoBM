// Fill out your copyright notice in the Description page of Project Settings.


#include "AIBot.h"

#include "NavigationSystem.h"
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
	ReceiveMoveCompleted.AddDynamic(this, &AAIBot::OnMoveCompletedCallback);
	
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

void AAIBot::TargetUpdate(AActor* SeenActor, FAIStimulus const Stim)
{
	UE_LOG(LogTemp, Warning, TEXT("Updated Perception"));
	if(Cast<ATarget>(SeenActor) && Stim.WasSuccessfullySensed())
	{
		UE_LOG(LogTemp, Warning, TEXT("Seen Target"));
		FVector HeadLocation = Self->HeadDirectionsComp->GetComponentLocation();
		FVector TargetLocation = SeenActor->GetActorLocation();
		FVector Direction = (TargetLocation - HeadLocation).GetSafeNormal();
		FRotator NewRotation = Direction.Rotation();
		Self->HeadDirectionsComp->SetWorldRotation(NewRotation);

		FVector Start = HeadLocation;
		FVector End = Start + Self->HeadDirectionsComp->GetForwardVector() * 1000; // 1000 is the distance of the raycast

		
		FHitResult HitResult;

		
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start,     End,      ECC_Visibility, FCollisionQueryParams(FName(TEXT("")), false, this) );

		
		DrawDebugLine(GetWorld(), Start, End, FColor::Green, false,  5.0f,   0,      1.0f    );
			
		SetFocus(SeenActor);
		Self->AIRifle->ToggleFire(true, SeenActor);
		
	}
	else
	{
		
		UE_LOG(LogTemp, Warning, TEXT("No Target"));
		Self->AIRifle->ToggleFire(false, nullptr);
		SetFocus(nullptr);
	}
}

void AAIBot::FollowAIPath()
{
	MoveToLocation(AiPath->GetPatrolPoint(3));
	UE_LOG(LogTemp, Warning, TEXT("Has Moved"));
}

void AAIBot::OnMoveCompletedCallback( FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	CurrentPathIndex++;
	UE_LOG(LogTemp, Warning, TEXT("Has Called Back"));
	FollowAIPath();
}







