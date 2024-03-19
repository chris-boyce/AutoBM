// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GunRangeButton.h"
#include "Target.h"
#include "GameFramework/Actor.h"
#include "GunRangeController.generated.h"

class UTextRenderComponent;
class AGunRangeSpawnPoint;
UCLASS()
class AUTOBM_API AGunRangeController : public AActor
{
	GENERATED_BODY()
	
public:	
	AGunRangeController();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	TArray<AGunRangeSpawnPoint*> SpawnPoints;

	UPROPERTY(VisibleAnywhere)
	float SpawnInterval = 3.0f;

	UPROPERTY(EditAnywhere)
	AActor* KillText;

	UPROPERTY(EditAnywhere)
	AActor* DenominatorKillText;
	
	UFUNCTION()
	void SpawnEnemyAtRandomPoint();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATarget> TargetClass;

	UFUNCTION()
	void KillCounter();

	UPROPERTY(VisibleAnywhere)
	int KillCount;

	UFUNCTION()
	void StartGunRange();

	UPROPERTY()
	int SpawnCount = 0;

	UPROPERTY()
	FTimerHandle SpawnTimerHandle;

	UPROPERTY(EditAnywhere)
	int SpawnAmount = 10;

	UPROPERTY(EditAnywhere)
	AGunRangeButton* EasyButton;

	UPROPERTY(EditAnywhere)
	AGunRangeButton* MediumButton;

	UPROPERTY(EditAnywhere)
	AGunRangeButton* HardButton;

	UPROPERTY(EditAnywhere)
	AGunRangeButton* StartButton;

	UFUNCTION()
	void ChangeDifficulty(float Interval);

	UFUNCTION()
	void ResetGunRange();
	

};
