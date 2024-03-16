// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponUpdateAmmoHUD, int, CO, int, FO);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponUpdateReloadTime, float, ReloadTime);

class UNiagaraComponent;
class ATracer;
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
	
	void StartFiring(); //Mouse Down
	void StopFiring(); //Mouse Up
	void Reload(); //R Key
	
	FWeaponFired WeaponFired; //Anim Event To First Person Player
	FWeaponReload WeaponReload;
	
	UPROPERTY()
	FWeaponUpdateAmmoHUD WeaponUpdateAmmoHUD;

	UPROPERTY()
	FWeaponUpdateReloadTime WeaponUpdateReloadTime;

	AFirstPersonPlayer* Player = nullptr; //Player Reference Set in Player

	

	
private:
	

	void BloodSplatter(FVector ImpactPoint);
	void FireWeapon();
	
	FVector ApplySprayPattern(FVector OriginalDirection); //Math Offsets for spray and accuarcy 
	FVector ApplyInaccuracy(FVector Direction, float Speed);
	
	FTimerHandle AutomaticFireTimer; 
	
	float FireRate = 0.1f; 
	
	int CurrentPatternIndex = 0; //Current spray number resets on stop fire

	float MaxWalkingInaccuracy = 10.0f; //TBD Speed before inaccurays

	UPROPERTY(EditAnywhere, Category = "Bullet") //Decals and Tracers
	UMaterialInterface* BulletDecalMaterial;

	UPROPERTY(EditAnywhere, Category = "Bullet") //Decals and Tracers
	UMaterialInterface* BulletDecalMaterialBlood;

	UPROPERTY(EditDefaultsOnly, Category = "Bullet")
	TSubclassOf<ATracer> BulletClass;

	UPROPERTY(EditDefaultsOnly, Category = "Blood")
	TSubclassOf<AActor> BloodParticalClass;

	
	
	UFUNCTION()
	void SpawnDecalAtLocation(FVector& Location, FVector& Normal, bool isTarget);

	
	UPROPERTY() //Ammo and Reload Properties 
	int FullAmmo = 30;
	
	UPROPERTY()
	int CurrentAmmo = 30;
	
	UFUNCTION()
	void RefillAmmo();
	
	UPROPERTY()
	float WeaponReloadTime = 2.0f;
	
	UPROPERTY()
	bool bIsReloading = false;

	UPROPERTY()
	float RunningOffsetDegreeClamp = 10.0f; //Spread Clamp Value
	
	//Player Controller for camera
	UPROPERTY()
	APlayerController* PlayerController = nullptr;

	//Barrel Location for muzzle flash and tracer
	UPROPERTY()
	FVector BarrelLocation;

	//End Pos for hit or endpos for not hit for tracer
	UPROPERTY()
	FVector TargetPosition;

	//Spray Pattern Data Table Values Written to array
	UPROPERTY(EditAnywhere)
	UDataTable* SprayPatternDataTable;

	UPROPERTY(EditAnywhere)
	TArray<FSprayPatternData> SprayPattern;
	
	//Unreal Memes Needs to do this for some reason
	UNiagaraComponent* GetNiagaraComponentByName(FName ComponentName);

	//Visuals
	UFUNCTION()
	void Tracers();

	UFUNCTION()
	void TurnOffMuzzleFlashLight();
	
	UFUNCTION()
	void MuzzleFlash();

	UPROPERTY(VisibleAnywhere) 
	UNiagaraComponent* MuzzleFlashSystem;

	UPointLightComponent* LightComponent;

	UPROPERTY(EditAnywhere, Category="Damage")
	FDamageInfo DamageInfo;

	

	
	

	

};
