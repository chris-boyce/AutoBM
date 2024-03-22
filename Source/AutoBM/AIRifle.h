// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Rifle.h"
#include "GameFramework/Actor.h"
#include "AIRifle.generated.h"

UCLASS()
class AUTOBM_API AAIRifle : public AActor
{
	GENERATED_BODY()
	
public:	
	AAIRifle();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void GunFiring(AActor* Target);

	UFUNCTION()
	void GunStopFiring();

	FTimerHandle FiringTimerHandle;

	UFUNCTION()
	void FireWeapon();

	UPROPERTY()
	AActor* CurrentActor;

	UPROPERTY(EditAnywhere, Category="Damage")
	FDamageInfo DamageInfo;

};
