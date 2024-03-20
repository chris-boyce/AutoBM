// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Walk.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTTask_Walk::UBTTask_Walk(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "FollowRoute";
}

EBTNodeResult::Type UBTTask_Walk::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	if(AAIController* const AIController = Cast<AAIController>(OwnerComp.GetAIOwner()))
	{
		
		if(APawn* const AIPawn = AIController->GetPawn())
		{
			FVector CurrentLocation = AIPawn->GetActorLocation();
			if(UNavigationSystemV1* const NavMesh = UNavigationSystemV1::GetCurrent(GetWorld()))
			{
				FNavLocation NewLocation;
				if(NavMesh->GetRandomPointInNavigableRadius(CurrentLocation, radius, NewLocation))
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), NewLocation.Location);
				}
				
				UE_LOG(LogTemp, Warning, TEXT("Successs"));
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return  EBTNodeResult::Succeeded;
			}
		}
			
	}
	return EBTNodeResult::Failed;
	
}
