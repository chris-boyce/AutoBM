// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_FindAIPathPoint.h"

#include "AIController.h"
#include "Target.h"
#include "BehaviorTree/BlackboardComponent.h"


UMyBTTask_FindAIPathPoint::UMyBTTask_FindAIPathPoint(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("FindAIPathPoint");
}

EBTNodeResult::Type UMyBTTask_FindAIPathPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if(AAIController* const SelfController = Cast<AAIController>(OwnerComp.GetAIOwner()))
	{
		if(UBlackboardComponent* const Blackboard = OwnerComp.GetBlackboardComponent())
		{
			int const index = Blackboard->GetValueAsInt(GetSelectedBlackboardKey());

			if(ATarget* AIPawn = Cast<ATarget>(SelfController->GetPawn()))
			{
				FVector Point = AIPawn->GetAIPath()->GetPatrolPoint(index);
				auto GlobalPoint = AIPawn->GetAIPath()->GetActorTransform().TransformPosition(Point);
				Blackboard->SetValueAsVector(AIPathVectorKey.SelectedKeyName, GlobalPoint);

				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return  EBTNodeResult::Succeeded;
				
			}
		}
	}
	return  EBTNodeResult::Failed;
}
