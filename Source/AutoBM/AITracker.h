// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIBot.h"
#include "Components/ActorComponent.h"
#include "AITracker.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AUTOBM_API UAITracker : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAITracker();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void StartTracking(AAIBot* AIComp,ATarget* Bot, AAIRifle* AIRifle);

	UFUNCTION()
	void MissedBullet();

	UFUNCTION()
	void HitHead();

	UFUNCTION()
	void HitBody();

	UFUNCTION()
	void HitOther();

	UFUNCTION()
	void SeenTimer();

	UFUNCTION()
	void KilledTimerStop();

	UFUNCTION()
	void TimeToDamageStop();

	float StartTime;

	float HitTime;

	bool FirstHit = true;

	TArray<float> KilledTime;

	TArray<float> TimeToDamage;

	int TotalShot;

	int HeadShot;

	int BodyShot;

	int OtherShot;

	int MissedShot;

	FString BotName;

	bool HasFinished = false;

	UFUNCTION()
	void OnCompletedCourse();

	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

		
};
