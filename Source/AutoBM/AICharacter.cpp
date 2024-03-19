// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"

#include "FirstPersonPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

void AAICharacter::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (BehaviorTree)
	{
		UE_LOG(LogTemp, Warning, TEXT("We Are Alive"));
		RunBehaviorTree(BehaviorTree);
	}
	
}

AAICharacter::AAICharacter(FObjectInitializer const& ObjectInitializer)
{
	SetupSight();
}

void AAICharacter::SetupSight()
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
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AAICharacter::OnTargetSeen);
		UE_LOG(LogTemp, Warning, TEXT("Add Dynamic"));
		GetPerceptionComponent()->ConfigureSense(*SenseSight);
		
	}
	
}

void AAICharacter::OnTargetSeen(AActor* SeenActor, FAIStimulus const Stim)
{
	UE_LOG(LogTemp, Warning, TEXT("SEEN PLAYER"));
	if(AFirstPersonPlayer* const Player = Cast<AFirstPersonPlayer>(SeenActor))
	{
		GetBlackboardComponent()->SetValueAsBool("bSeenPlayer", Stim.WasSuccessfullySensed());
	}
}


