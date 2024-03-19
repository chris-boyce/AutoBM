// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "AICharacter.generated.h"

class UAISenseConfig_Sight;
UCLASS()
class AUTOBM_API AAICharacter : public AAIController
{
	
	
	GENERATED_BODY()
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(EditAnywhere)
	UBehaviorTree* BehaviorTree;
	
public:

	explicit AAICharacter(FObjectInitializer const& ObjectInitializer);

	UPROPERTY(EditAnywhere)
	UAISenseConfig_Sight* SenseSight;

	UFUNCTION()
	void SetupSight();

	UFUNCTION()
	void OnTargetSeen(AActor* SeenActor, FAIStimulus const Stim);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	float SightRadius = 3000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	float LoseRadius = 5000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	float VisionAngle = 180;

	
};
