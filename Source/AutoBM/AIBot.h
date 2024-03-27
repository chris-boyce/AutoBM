// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Target.h"
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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI|Stats")
	float SightRadius = 3000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI|Stats")
	float LoseRadius = 5000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI|Stats")
	float VisionAngle = 360;

	UPROPERTY()
	ATarget* Self;

	UFUNCTION()
	void KilledTarget();

	bool bIntentionallyStoppedMovement = false;

	FTimerHandle WalkingReactionTimerHandle;

	FTimerHandle FiringReactionTimerHandle;

	UPROPERTY(EditAnywhere, Category="AI|Stats")
	float WalkingReactionLowerBound = 0.3f;

	UPROPERTY(EditAnywhere, Category="AI|Stats")
	float WalkingReactionUpperBound = 0.7f;

	UFUNCTION()
	void StopMovementAfterDelay();

	UPROPERTY(EditAnywhere, Category="AI|Stats")
	float FiringReactionLowerBound = 0.4f;

	UPROPERTY(EditAnywhere, Category="AI|Stats")
	float FiringReactionUpperBound = 0.7f;

	UFUNCTION()
	void StartFiringAfterDelay();

	UPROPERTY()
	AActor* TempActor;

	UPROPERTY()
	TArray<AActor*> CurrentlySensedActors;
	
	UPROPERTY(EditAnywhere, Category="AI|Stats")
	UCurveFloat* AimCurve;

	UPROPERTY(EditAnywhere, Category="AI|Stats")
	UCurveFloat* VarietyCurve;

	UFUNCTION()
	void RecoilDivision(int CurrentSprayBullet);

	UPROPERTY(EditAnywhere, Category="AI|Stats")
	float HeadshotPercentageAim = 25.0f;

	UPROPERTY(EditAnywhere, Category="AI|Stats")
	int BulletMissedResetAmount = 3;

	UPROPERTY(EditAnywhere, Category="AI|Stats")
	float TimeForAimToResetAfterMissed = 1.0f;

	UFUNCTION()
	FVector CalculateAimTarget();

	UFUNCTION()
	void BulletMissedResetAim();

	UPROPERTY()
	int BulletMissCount;

	UFUNCTION()
	void InitializeController(float HeadShotPercentage, int BulletMissResetCount, float AimResetSpeed, UCurveFloat* AimCurves, UCurveFloat* VarCurve, float FiringReactionLower, float FiringReactionUpper, float WalkingReactionLower, float WalkingReactionUpper );
	
};
