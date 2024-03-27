// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Rifle.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "AIRifle.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRecoilDiv, int, CurrentSprayIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBulletMissed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBulletHit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHeadshot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBodyShot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOtherShot);

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
	void ToggleFire(bool Toggle, FVector Target);
	
	FVector ApplySprayPattern(FVector Vector);

	FVector ApplyInaccuracy(FVector Vector);
	void FireBullet();

	UPROPERTY(VisibleAnywhere)
	FVector CurrentTarget;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* HeadDirection;

	FTimerHandle FireRateTimer;

	UPROPERTY(EditDefaultsOnly, Category="Gun|Stats") //Spray Pattern Data Table Values Written to array
	UDataTable* SprayPatternDataTable;

	UPROPERTY() 
	TArray<FSprayPatternData> SprayPattern;

	int CurrentPatternIndex = 0;

	int FullAmmo = 30;
	int CurrentAmmo = 30;

	void Reload();

	float RecoilDivider = 1;

	FRecoilDiv RecoilDiv;

	FBulletMissed BulletMissed;

	FBulletHit BulletHit;

	FHeadshot Headshot;

	FBodyShot BodyShot;

	FOtherShot OtherShot;

};
