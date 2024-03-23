// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Target.h"
#include "Perception/AIPerceptionComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "NavigationSystem.h"
#include "AIBot.generated.h"


UCLASS()
class AUTOBM_API AAIBot : public AAIController
{
	GENERATED_BODY()

	virtual void OnPossess(APawn* InPawn) override;

	virtual void BeginPlay() override;

	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

public:
	explicit AAIBot(FObjectInitializer const& ObjectInitializer);

	UPROPERTY()
	UAISenseConfig_Sight* SenseSight;

	UFUNCTION()
	void SetupSight();
	
	
	
	UFUNCTION()
	void TargetUpdate(AActor* SeenActor, FAIStimulus const Stim);

	UFUNCTION()
	void FollowAIPath();

	UFUNCTION()
	void ContinuePath();
	
	UPROPERTY()
	int CurrentPathIndex = 0;;

	UPROPERTY()
	AAIPath* AiPath;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	float SightRadius = 3000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	float LoseRadius = 5000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	float VisionAngle = 360;

	UPROPERTY()
	ATarget* Self;

	UFUNCTION()
	void KilledTarget();

	bool bIntentionallyStoppedMovement = false;

	FTimerHandle WalkingReactionTimerHandle;

	FTimerHandle FiringReactionTimerHandle;

	UPROPERTY(EditAnywhere)
	float WalkingReactionLowerBound = 0.5f;

	UPROPERTY(EditAnywhere)
	float WalkingReactionUpperBound = 1.0f;

	UFUNCTION()
	void StopMovementAfterDelay();

	UPROPERTY(EditAnywhere)
	float FiringReactionLowerBound = 0.4f;

	UPROPERTY(EditAnywhere)
	float FiringReactionUpperBound = 0.7f;

	UFUNCTION()
	void StartFiringAfterDelay();

	UPROPERTY()
	AActor* TempActor;
	
	
	
	
};
