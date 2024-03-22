// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_LookAtTarget.h"

#include "AICharacter.h"
#include "AIController.h"
#include "Target.h"
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
		
		FVector Start = SelfController->GetPawn()->GetActorLocation();
		FVector ForwardVector = SelfController->GetPawn()->GetActorForwardVector(); 
		FVector End = Start + (ForwardVector * 1000.0f); 
		
		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(SelfController); 
		
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams);
		
		if(bHit)
		{
			UE_LOG(LogTemp, Warning, TEXT("Actor Name: %s"), *HitResult.GetActor()->GetName());
			DrawDebugLine(GetWorld(),Start,End,FColor::Green, false, 5.0f, 0, 1.0f );
			if(Cast<ATarget>(HitResult.GetActor()))
			{
				UE_LOG(LogTemp, Warning, TEXT("Has Hit the targett"))
				SelfController->GetBlackboardComponent()->SetValueAsBool("bLookingAtTarget", true);
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
			return EBTNodeResult::Failed;
		}
		
	}
	return EBTNodeResult::Failed;
	
}
