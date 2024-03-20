// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Fire.h"

#include "AICharacter.h"

UBTTask_Fire::UBTTask_Fire(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "FireWeapon";
}

EBTNodeResult::Type UBTTask_Fire::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAICharacter* SelfController = Cast<AAICharacter>(OwnerComp.GetAIOwner());
	if(SelfController)
	{
		SelfController->FireWeapon();
	}
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
