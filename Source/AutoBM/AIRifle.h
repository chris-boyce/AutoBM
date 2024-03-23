// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Rifle.h"
#include "Components/BoxComponent.h"
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
	
	FTimerHandle FiringTimerHandle;
	
	UPROPERTY(EditAnywhere, Category="Damage")
	FDamageInfo DamageInfo;

	void FireGun();
	void StopFireGun();
	void ToggleFire(bool Toggle, AActor* Target);
	void FireBullet();

	UPROPERTY(VisibleAnywhere)
	AActor* CurrentActor;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* HeadDirection;

	FTimerHandle FireRateTimer;

};
