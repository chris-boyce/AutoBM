// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Fire.generated.h"

/**
 * 
 */
UCLASS()
class AUTOBM_API UBTTask_Fire : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	explicit UBTTask_Fire(FObjectInitializer const& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
