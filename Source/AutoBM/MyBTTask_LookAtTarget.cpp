// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_LookAtTarget.h"

#include "AICharacter.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UMyBTTask_LookAtTarget::UMyBTTask_LookAtTarget(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "LookAtTarget";
}

EBTNodeResult::Type UMyBTTask_LookAtTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAICharacter* SelfController = Cast<AAICharacter>(OwnerComp.GetAIOwner());
	if(SelfController)
	{
		SelfController->SetFocus(SelfController->Target);
		SelfController->GetBlackboardComponent()->SetValueAsBool("bLookingAtTarget", true);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return  EBTNodeResult::Succeeded;

	}
	return EBTNodeResult::Failed;
	
}
