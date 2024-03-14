// Fill out your copyright notice in the Description page of Project Settings.


#include "Rifle.h"

#include "Components/DecalComponent.h"
#include "FirstPersonPlayer.h"
#include "NiagaraComponent.h"
#include "Tracer.h"
#include "Components/PointLightComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

ARifle::ARifle()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ARifle::BeginPlay()
{
	Super::BeginPlay();

	MuzzleFlashSystem = GetNiagaraComponentByName("MuzzleSystem");
	LightComponent= FindComponentByClass<UPointLightComponent>();
	LightComponent->SetIntensity(0.0f);
	
	if (SprayPatternDataTable)
	{
		TArray<FName> RowNames = SprayPatternDataTable->GetRowNames();
		
		for (const FName& RowName : RowNames)
		{
			FSprayPatternData* RowData = SprayPatternDataTable->FindRow<FSprayPatternData>(RowName, TEXT(""));

			if (RowData)
			{
				SprayPattern.Add(*RowData);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("THIS IS NOT FOUND"));
	}
	
}

void ARifle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

}

void ARifle::StartFiring()
{
	FireWeapon();
	GetWorld()->GetTimerManager().SetTimer(AutomaticFireTimer, this, &ARifle::FireWeapon, FireRate, true);
}

void ARifle::StopFiring()
{
	GetWorld()->GetTimerManager().ClearTimer(AutomaticFireTimer);
	CurrentPatternIndex = 0;
}



void ARifle::MuzzleFlash()
{
	MuzzleFlashSystem->ActivateSystem(true);
	LightComponent->SetIntensity(2000.0f); 
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ARifle::TurnOffMuzzleFlashLight, 0.25f, false);
}

void ARifle::FireWeapon()
{
	if(bIsReloading) //Checks Player Reload
	{
		UE_LOG(LogTemp, Warning, TEXT("Reloading Stopped"));
		return;
	}
	
	if (!PlayerController)//Checks Has Player Character
	{
		PlayerController = GetWorld()->GetFirstPlayerController();
	}
	
	WeaponFired.Broadcast(); //Broadcast Anim To Player //FirstPersonPlayer
	CurrentAmmo--;

	if(CurrentAmmo == 0) //Check Ammo
	{
		Reload();
	}
	
	FVector StartLocation; //Calculation for Firing Direction
	FRotator StartRotation;
	
	PlayerController->GetPlayerViewPoint(StartLocation, StartRotation);

	FVector FiringDirection = StartRotation.Vector();
	
	FiringDirection = ApplySprayPattern(FiringDirection);
	
	
	if(!Player->bIsCrouched) //If Player is not Crouch it will calculate inaccuracy 
	{
		float PlayerSpeed = 0.0f;
		PlayerSpeed = Player->GetVelocity().Size();
		FiringDirection = ApplyInaccuracy(FiringDirection, PlayerSpeed);
	}
	

	FVector EndLocation = StartLocation + (FiringDirection * 10000); 
	
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); 
	QueryParams.AddIgnoredActor(GetOwner()); 
	QueryParams.bTraceComplex = true;

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams);

	if (bHit)
	{
		SpawnDecalAtLocation(HitResult.ImpactPoint, HitResult.ImpactNormal);
		TargetPosition = HitResult.ImpactPoint;
		AActor* HitActor = HitResult.GetActor();
		if (HitActor)
		{
			//When Hit Actor
		}
		
	}
	else
	{
		TargetPosition = EndLocation;
	}
	
	Tracers();
	MuzzleFlash();

	WeaponUpdateAmmoHUD.Broadcast(CurrentAmmo,FullAmmo);
	
	//DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 1.0f, 0, 1.0f);
}

FVector ARifle::ApplySprayPattern(FVector OriginalDirection)
{
	FVector2D Pattern = SprayPattern[CurrentPatternIndex].SpreadFactor;
	CurrentPatternIndex = CurrentPatternIndex + 1;

	FRotator DirectionRotator = OriginalDirection.Rotation();
	DirectionRotator.Yaw += Pattern.X;
	DirectionRotator.Pitch += Pattern.Y;

	FVector NewDirection = DirectionRotator.Vector();
	
	UE_LOG(LogTemp, Warning, TEXT("New Direction Vector: X=%f, Y=%f, Z=%f"), NewDirection.X, NewDirection.Y, NewDirection.Z);

	return NewDirection;
}

FVector ARifle::ApplyInaccuracy(FVector Direction, float Speed)
{
	float PlayerSpeed = Player->GetMovementSpeed();
	float ClampedSpeed = FMath::Clamp(PlayerSpeed, 0.0f, RunningOffsetDegreeClamp);
	float InaccuracyAmount = ClampedSpeed;
	float InaccuracyYaw = FMath::RandRange(-InaccuracyAmount, InaccuracyAmount);
	float InaccuracyPitch = FMath::RandRange(-InaccuracyAmount, InaccuracyAmount);

	FRotator DirectionRotator = Direction.Rotation();
	DirectionRotator.Yaw += InaccuracyYaw;
	DirectionRotator.Pitch += InaccuracyPitch;

	FVector NewDirection = DirectionRotator.Vector();
	
	return NewDirection;
}

void ARifle::SpawnDecalAtLocation(FVector& Location, FVector& Normal)
{
	if (BulletDecalMaterial)
	{
		UDecalComponent* Decal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), BulletDecalMaterial, FVector(10.0f, 10.0f, 10.0f), Location, Normal.Rotation());
		Decal->SetFadeScreenSize(0.001f); 
	}
}

void ARifle::RefillAmmo()
{
	CurrentAmmo = FullAmmo;
	bIsReloading = false;
	WeaponUpdateAmmoHUD.Broadcast(CurrentAmmo,FullAmmo);
	UE_LOG(LogTemp, Warning, TEXT("RELOAD FINISHED"));
}

UNiagaraComponent* ARifle::GetNiagaraComponentByName(FName ComponentName)
{
	TArray<UActorComponent*> Components;
	GetComponents(Components);

	for (UActorComponent* Component : Components)
	{
		UNiagaraComponent* NiagaraComponent = Cast<UNiagaraComponent>(Component);
		if (NiagaraComponent && NiagaraComponent->GetName() == ComponentName.ToString())
		{
			return NiagaraComponent;
		}
	}
	return nullptr;
}

void ARifle::Tracers()
{
	USkeletalMeshComponent* Mesh = FindComponentByClass<USkeletalMeshComponent>();
	if(Mesh->DoesSocketExist("Barrel"))
	{
		BarrelLocation = Mesh->GetSocketLocation("GunSocket");
		UE_LOG(LogTemp, Warning, TEXT("Barrel Location Set"));
		UE_LOG(LogTemp, Warning, TEXT("Barrel Location: %s"), *BarrelLocation.ToString());
	}
	
	FVector Direction = (TargetPosition - BarrelLocation).GetSafeNormal();

	UE_LOG(LogTemp, Warning, TEXT("Barrel Location: %s"), *BarrelLocation.ToString());
	
	ATracer* Projectile = GetWorld()->SpawnActor<ATracer>(BulletClass, BarrelLocation, Direction.Rotation());

	if (Projectile)
	{
		Projectile->StartMovement(Direction);
	}
	
	
}

void ARifle::TurnOffMuzzleFlashLight()
{
	LightComponent->SetIntensity(0.0f);
}

void ARifle::Reload()
{
	
	if(bIsReloading || CurrentAmmo == FullAmmo)
	{
		return;
	}
	
	FTimerHandle AmmoRefillTimerHandle;
	WeaponUpdateReloadTime.Broadcast(WeaponReloadTime);
	bIsReloading = true;
	WeaponReload.Broadcast();
	GetWorld()->GetTimerManager().SetTimer(AmmoRefillTimerHandle, this, &ARifle::RefillAmmo, WeaponReloadTime, false);
	
	
}

