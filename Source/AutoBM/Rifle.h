// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickupable.h"
#include "GameFramework/Actor.h"
#include "Rifle.generated.h"

class UPointLightComponent;
USTRUCT(BlueprintType) //Spray Pattern Struct For Data Table
struct FSprayPatternData : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector2D SpreadFactor;
};

USTRUCT(BlueprintType)
struct FDamageInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HeadshotDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BodyShotDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AppendageDamage;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWeaponFired); //Binds to Gun for anim calls
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWeaponReload);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponUpdateAmmoHUD, int, CO, int, FO); //Binds to HUD
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponUpdateReloadTime, float, ReloadTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerBulletMissed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerBulletHit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerHeadshot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerBodyShot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerOtherShot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerFiringMoving);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerFiringStopped);

class UNiagaraComponent;
class ATracer;
class AFirstPersonPlayer;
UCLASS()
class AUTOBM_API ARifle : public AActor, public IPickupable
{
	GENERATED_BODY()
	
public:	
	ARifle();

protected:
	virtual void BeginPlay() override;
public:
	
	virtual void Tick(float DeltaTime) override;
	
	void StartFiring(); //Mouse Down
	void StopFiring(); //Mouse Up
	void Reload(); //R Key
	
	FWeaponFired WeaponFired; //Anim Event To First Person Player
	FWeaponReload WeaponReload;
	
	UPROPERTY()
	FWeaponUpdateAmmoHUD WeaponUpdateAmmoHUD;

	UPROPERTY()
	FWeaponUpdateReloadTime WeaponUpdateReloadTime;

	UPROPERTY()
	AFirstPersonPlayer* Player = nullptr; //Player Reference Set in Player

	virtual UClass* Pickup() override;

	UPROPERTY(EditDefaultsOnly, Category="Gun|Stats") 
	int FullAmmo = 30;
	
	UPROPERTY(EditDefaultsOnly, Category="Gun|Stats") 
	int CurrentAmmo = 30;

	FPlayerBulletHit BulletHit;

	FPlayerBulletMissed BulletMissed;

	FPlayerHeadshot Headshot;

	FPlayerBodyShot BodyShot;

	FPlayerOtherShot OtherShot;

	FPlayerFiringMoving FiringMoving;

	FPlayerFiringStopped FiringStopped;
	
private:
	// Gun Functionality Functions ------------
	void FireWeapon();
	
	FVector ApplySprayPattern(FVector OriginalDirection); //Math Offsets for spray and accuarcy 
	FVector ApplyInaccuracy(FVector Direction, float Speed);

	UFUNCTION()
	void RefillAmmo();

	// Gun Visual Functions ------------
	
	UNiagaraComponent* GetNiagaraComponentByName(FName ComponentName);
	
	UFUNCTION()
	void Tracers();

	UFUNCTION()
	void TurnOffMuzzleFlashLight();
	
	UFUNCTION()
	void MuzzleFlash();

	void BloodSplatter(FVector ImpactPoint);

	UFUNCTION()
	void SpawnDecalAtLocation(FVector& Location, FVector& Normal, bool isTarget);

	// Gun Stats ------------
	
	UPROPERTY(EditDefaultsOnly, Category="Gun|Stats")
	float FireRate = 0.1f;

	UPROPERTY(EditDefaultsOnly, Category="Gun|Stats") 
	float WeaponReloadTime = 2.0f;
	
	UPROPERTY(EditDefaultsOnly, Category="Gun|Stats") //Spread Clamp Value
	float RunningOffsetDegreeClamp = 10.0f; 
	
	UPROPERTY(EditDefaultsOnly, Category="Gun|Stats") //Spray Pattern Data Table Values Written to array
	UDataTable* SprayPatternDataTable;

	UPROPERTY(EditDefaultsOnly, Category="Gun|Stats") 
	FDamageInfo DamageInfo;

	int CurrentPatternIndex = 0; //Current spray number resets on stop fire
	
	FTimerHandle AutomaticFireTimer; 
	
	UPROPERTY()
	bool bIsReloading = false;
	
	UPROPERTY() //Player Controller for camera
	APlayerController* PlayerController = nullptr;

	UPROPERTY() 
	TArray<FSprayPatternData> SprayPattern;

	// Gun Visuals Variables ------------
	
	UPROPERTY(EditDefaultsOnly, Category="Gun|Visuals|Sounds") 
	USoundBase* GunFire;

	UPROPERTY(EditAnywhere, Category = "Gun|Visuals|Bullet") //Decals and Tracers
	UMaterialInterface* BulletDecalMaterial;

	UPROPERTY(EditAnywhere, Category = "Gun|Visuals|Blood") //Decals and Tracers
	UMaterialInterface* BulletDecalMaterialBlood;

	UPROPERTY(EditDefaultsOnly, Category = "Gun|Visuals|Bullet")
	TSubclassOf<ATracer> BulletClass;

	UPROPERTY(EditAnywhere, Category = "Gun|Visuals|Blood")
	TSubclassOf<AActor> BloodParticalClass;

	UPROPERTY() //Barrel Location for muzzle flash and tracer
	FVector BarrelLocation;
	
	UPROPERTY() //End Pos for hit or endpos for tracer
	FVector TargetPosition;
	
	UPROPERTY() 
	UNiagaraComponent* MuzzleFlashSystem;

	UPROPERTY()
	UPointLightComponent* LightComponent;
	
	
	
};
