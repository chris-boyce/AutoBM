// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"
#include "Target.h"
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
	if(Stim.WasSuccessfullySensed())
	{
		if(Cast<ATarget>(SeenActor))
		{
			Target = SeenActor;
			UE_LOG(LogTemp, Warning, TEXT("Target"));
			GetBlackboardComponent()->SetValueAsBool("bSeenTarget", Stim.WasSuccessfullySensed());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("LOST THE ACTOR"));
		GetBlackboardComponent()->SetValueAsBool("bSeenTarget", false);
		GetBlackboardComponent()->SetValueAsBool("bLookingAtTarget", false);
		SetFocus(nullptr);
	}
	
}



void AAICharacter::FireWeapon()
{
	UE_LOG(LogTemp, Warning, TEXT("Firing Weapon"));
}




