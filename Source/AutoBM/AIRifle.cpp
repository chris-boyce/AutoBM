// Fill out your copyright notice in the Description page of Project Settings.


#include "AIRifle.h"

#include "Shootable.h"

AAIRifle::AAIRifle()
{
	PrimaryActorTick.bCanEverTick = true;

}


void AAIRifle::BeginPlay()
{
	Super::BeginPlay();
	
}


void AAIRifle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAIRifle::GunFiring(AActor* Target)
{
	if (!Target) return;
	UE_LOG(LogTemp, Warning, TEXT("Hasnt Returned"));
	CurrentActor = Target;
	GetWorld()->GetTimerManager().SetTimer(FiringTimerHandle, this, &AAIRifle::FireWeapon, 0.2f, true);
}



void AAIRifle::GunStopFiring()
{
	UE_LOG(LogTemp, Warning, TEXT("Firing Stop Weapon"));
	CurrentActor = nullptr;
	GetWorld()->GetTimerManager().ClearTimer(FiringTimerHandle);
}

void AAIRifle::FireWeapon()
{
	if(!CurrentActor) return;
	UE_LOG(LogTemp, Warning, TEXT("Firing Weapon"));
	FVector StartLocation = GetActorLocation();
	FVector EndLocation = CurrentActor->GetActorLocation();
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	FHitResult HitResult;
	
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, CollisionParams);
	DrawDebugLine(GetWorld(),StartLocation,EndLocation, FColor::Red,  false,5.0f,0,1.0f);
	if (bHit)
	{
		auto HitComponent = HitResult.GetComponent();
		if(HitComponent)
		{
			AActor* HitActor = HitResult.GetActor();
			IShootable* HitHandler = Cast<IShootable>(HitActor);
			if(HitHandler)
			{
				HitHandler->HandleHit(HitComponent, DamageInfo);
			}
		}
	}
}

