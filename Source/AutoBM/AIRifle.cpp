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
	HeadDirection = GetComponentByClass<UBoxComponent>();
}


void AAIRifle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAIRifle::FireGun()
{
	GetWorld()->GetTimerManager().SetTimer(FireRateTimer, this, &AAIRifle::FireBullet, 0.25f, true);
}

void AAIRifle::StopFireGun()
{
	
}

void AAIRifle::ToggleFire(bool Toggle, AActor* Target)
{
	if(Toggle)
	{
		FireGun();
		CurrentActor = Target;
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(FireRateTimer);
	}
}

void AAIRifle::FireBullet()
{
	UE_LOG(LogTemp, Warning, TEXT("Fire Bullet"));
	FVector HeadLocation = HeadDirection->GetComponentLocation();
	if(!CurrentActor) return;
	FVector TargetLocation = CurrentActor->GetActorLocation();
	FVector Direction = (TargetLocation - HeadLocation).GetSafeNormal();
	FRotator NewRotation = Direction.Rotation();
	HeadDirection->SetWorldRotation(NewRotation);

	FVector Start = HeadLocation;
	FVector End = Start + HeadDirection->GetForwardVector() * 10000; // 1000 is the distance of the raycast

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); 
	QueryParams.AddIgnoredActor(GetOwner()); 
	QueryParams.bTraceComplex = true;
	
	FHitResult HitResult;
	
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult,Start,End, ECC_Visibility, QueryParams);

	if(bHit)
	{
		auto HitComponent = HitResult.GetComponent();
		if (HitComponent)
		{
			AActor* HitActor = HitResult.GetActor();
			IShootable* HitHandler = Cast<IShootable>(HitActor);
			if (HitHandler)
			{
				HitHandler->HandleHit(HitComponent, DamageInfo);
			}
			
		}
	}
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false,  5.0f,   0,      1.0f    );
	
}


