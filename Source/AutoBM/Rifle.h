// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Rifle.generated.h"

UCLASS()
class AUTOBM_API ARifle : public AActor
{
	GENERATED_BODY()
	
public:	
	ARifle();

protected:
	virtual void BeginPlay() override;

public:
	
	virtual void Tick(float DeltaTime) override;
	void StartFiring();
	void StopFiring();
private:
	
	void FireWeapon();
	FVector ApplySprayPattern(FVector OriginalDirection);
	FVector ApplyInaccuracy(FVector Direction, float Speed);

	FTimerHandle AutomaticFireTimer;
	
	float FireRate = 0.2f;

	TArray<FVector2D> SprayPattern;
	
	int CurrentPatternIndex = 0;

	float MaxWalkingInaccuracy = 10.0f; 

};
