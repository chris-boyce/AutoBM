// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Target.h"
#include "Components/ActorComponent.h"
#include "UserTracker.generated.h"


class UBoxComponent;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AUTOBM_API UUserTracker : public UActorComponent
{
	GENERATED_BODY()

public:	
	UUserTracker();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void StartTracking();

	void StopTracking();

	void UnbindDelegatesAndTimers();

	void ResetPlayerPos();
	
	UFUNCTION()
	void ViewPortChecker();

	UFUNCTION()
	void SubToTarget(ATarget* Target);

	UFUNCTION()
	void BotDeath(ATarget* Target);

	UFUNCTION()
	void BotDamage(ATarget* Target);

	UFUNCTION()
	void IsBeingBlocked();

	UPROPERTY(EditAnywhere)
	TSubclassOf<ATarget> TargetClass;

	UPROPERTY(VisibleAnywhere)
	TArray<ATarget*> SeenTargets;

	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> InSenseRadius;

	FTimerHandle LineTraceTimerHandle;

	FTimerHandle SphereTimerHandle;

	UPROPERTY()
	TMap<ATarget*, float> StartTimeMap;

	UPROPERTY()
	TMap<ATarget*, float> DamageTimeMap;

	UPROPERTY()
	TMap<ATarget*, float> DamageToKillTimeMap;

	void StartTimer(ATarget* Target);

	UPROPERTY(VisibleAnywhere)
	TArray<float> TimeToDamage;
	
	UPROPERTY(VisibleAnywhere)
	TArray<float> DamageToKillTime;
	
	UPROPERTY(VisibleAnywhere)
	TArray<float> KilledTime;

	UPROPERTY()
	ARifle* Gun;

	int TotalShot;

	int HeadShot;

	int BodyShot;

	int OtherShot;

	int MissedShot;

	int FireMovingCount;

	int FireStopCount;

	FString PlayerName = "Chris";

	UFUNCTION()
	void MissedBullet();

	UFUNCTION()
	void HitHead();

	UFUNCTION()
	void HitBody();

	UFUNCTION()
	void HitOther();

	UFUNCTION()
	void FiringWhenMoving();

	UFUNCTION()
	void FiringWhenStopped();
};
