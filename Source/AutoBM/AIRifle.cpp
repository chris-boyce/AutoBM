// Fill out your copyright notice in the Description page of Project Settings.


#include "AIRifle.h"

#include "Shootable.h"
#include "Target.h"

AAIRifle::AAIRifle()
{
	PrimaryActorTick.bCanEverTick = true;
}


void AAIRifle::BeginPlay()
{
	Super::BeginPlay();
	HeadDirection = GetComponentByClass<UBoxComponent>();

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
	GetWorld()->GetTimerManager().ClearTimer(FireRateTimer);
	CurrentPatternIndex = 0;
}

void AAIRifle::ToggleFire(bool Toggle, FVector Target)
{
	if(Toggle)
	{
		CurrentTarget = Target;
		FireGun();
	}
	else
	{
		StopFireGun();
	}
}

FVector AAIRifle::ApplySprayPattern(FVector Vector)
{
	RecoilDiv.Broadcast(CurrentPatternIndex);
	FVector2D Pattern = SprayPattern[CurrentPatternIndex].SpreadFactor;
	CurrentPatternIndex = CurrentPatternIndex + 1;

	FRotator DirectionRotator = Vector.Rotation();
	DirectionRotator.Yaw += Pattern.X / RecoilDivider;
	DirectionRotator.Pitch += Pattern.Y / RecoilDivider;

	FVector NewDirection = DirectionRotator.Vector();
	
	return NewDirection;
}

FVector AAIRifle::ApplyInaccuracy(FVector Vector)
{
	float Speed = Cast<ATarget>(GetOwner())->GetVelocity().Normalize();
	float ClampedSpeed = FMath::Clamp(Speed, 0.0f, 10.0f);
	float InaccuracyAmount = ClampedSpeed;
	float InaccuracyYaw = FMath::RandRange(-InaccuracyAmount, InaccuracyAmount);
	float InaccuracyPitch = FMath::RandRange(-InaccuracyAmount, InaccuracyAmount);

	FRotator DirectionRotator = Vector.Rotation();
	DirectionRotator.Yaw += InaccuracyYaw;
	DirectionRotator.Pitch += InaccuracyPitch;

	FVector NewDirection = DirectionRotator.Vector();
	
	return NewDirection;
}

void AAIRifle::FireBullet()
{
	if(CurrentAmmo <= 0)
	{
		Reload();
		return;
	}
	CurrentAmmo--;
	FVector HeadLocation = HeadDirection->GetComponentLocation();
	FVector TargetLocation = CurrentTarget;
	FVector Direction = (TargetLocation - HeadLocation).GetSafeNormal();
	FRotator NewRotation = Direction.Rotation();
	HeadDirection->SetWorldRotation(NewRotation);

	FVector Start = HeadLocation;
	FVector FiringDirection = HeadDirection->GetForwardVector();

	FiringDirection = ApplySprayPattern(FiringDirection);

	FiringDirection = ApplyInaccuracy(FiringDirection);
	
	FVector End = Start + FiringDirection * 10000; 

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
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Bullet Missed"));
				BulletMissed.Broadcast();
			}
		}
	}
	
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false,  5.0f,   0,      1.0f    );
	
}

void AAIRifle::Reload()
{
	CurrentAmmo = FullAmmo;
	CurrentPatternIndex = 0;
}


