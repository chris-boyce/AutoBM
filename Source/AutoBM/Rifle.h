// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Rifle.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWeaponFired);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWeaponReload);
class AFirstPersonPlayer;
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
	void Reload();

	UPROPERTY()
	float RunningOffsetDegreeClamp = 10.0f;

	FWeaponFired WeaponFired;

	FWeaponReload WeaponReload;

	AFirstPersonPlayer* Player = nullptr;
private:
	
	void FireWeapon();
	
	FVector ApplySprayPattern(FVector OriginalDirection);
	FVector ApplyInaccuracy(FVector Direction, float Speed);
	
	

	FTimerHandle AutomaticFireTimer;
	
	float FireRate = 0.2f;

	TArray<FVector2D> SprayPattern;
	
	int CurrentPatternIndex = 0;

	float MaxWalkingInaccuracy = 10.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Decal")
	UMaterialInterface* BulletDecalMaterial;
	
	UFUNCTION()
	void SpawnDecalAtLocation(FVector& Location, FVector& Normal);

	UPROPERTY()
	int FullAmmo = 30;

	UPROPERTY()
	int CurrentAmmo = 30;

	

	
	

	

};
