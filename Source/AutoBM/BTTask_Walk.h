// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Walk.generated.h"

UCLASS()
class AUTOBM_API UBTTask_Walk : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	explicit UBTTask_Walk(FObjectInitializer const& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "True"))
	float radius = 1000;
	
};
