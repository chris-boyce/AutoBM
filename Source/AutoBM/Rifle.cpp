// Fill out your copyright notice in the Description page of Project Settings.


#include "Rifle.h"

#include "GameFramework/Character.h"

ARifle::ARifle()
{
	PrimaryActorTick.bCanEverTick = true;
	SprayPattern = {
		FVector2D(0, 1),     // Start moving up
		FVector2D(0, 1.2),
		FVector2D(0.3, 1.2), // Start moving right
		FVector2D(0.6, 1.1),
		FVector2D(0.9, 1),   // More to the right
		FVector2D(-0.5, 1),  // Sharp left
		FVector2D(-0.7, 1),
		FVector2D(-0.4, 0.9),// Start tapering off left
		FVector2D(-0.2, 0.8),
		FVector2D(0.1, 0.7), // Slight right
		FVector2D(0.2, 0.7),
		FVector2D(0.3, 0.6), // More right
		FVector2D(0.1, 0.6), // Less right
		FVector2D(-0.1, 0.6),// Tiny left
		FVector2D(0, 0.5),   // Straight up
		FVector2D(0.2, 0.4), // Slight right
		FVector2D(-0.2, 0.4),// Slight left
		FVector2D(0.2, 0.3), // Back to right
		FVector2D(-0.2, 0.3),// Back to left
		FVector2D(0.1, 0.3), // Less right
		FVector2D(-0.1, 0.3),// Less left
		FVector2D(0, 0.3),   // Tiny up
		FVector2D(0.1, 0.2), // Slight right
		FVector2D(-0.1, 0.2),// Slight left
		FVector2D(0.1, 0.2), // Slight right
		FVector2D(-0.1, 0.2),// Slight left
		FVector2D(0, 0.2),   // Tiny up
		FVector2D(0.1, 0.1), // Slight right
		FVector2D(-0.1, 0.1),// Slight left
		FVector2D(0, 0.1),   // End with tiny up
	};

}

void ARifle::BeginPlay()
{
	Super::BeginPlay();
	
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
}

void ARifle::FireWeapon()
{
	UE_LOG(LogTemp, Warning, TEXT("Firing Weapon"));
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController) return;

	FVector StartLocation;
	FRotator StartRotation;
	PlayerController->GetPlayerViewPoint(StartLocation, StartRotation);

	FVector FiringDirection = StartRotation.Vector();


	FiringDirection = ApplySprayPattern(FiringDirection);
	float PlayerSpeed = 0.0f;
	if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
	{
		PlayerSpeed = OwnerCharacter->GetVelocity().Size();
	}
	FiringDirection = ApplyInaccuracy(FiringDirection, PlayerSpeed);

	FVector EndLocation = StartLocation + (FiringDirection * 10000); 
	
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); 
	QueryParams.AddIgnoredActor(GetOwner()); 
	QueryParams.bTraceComplex = true;

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams);

	if (bHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("HIT ENEMY"));
		AActor* HitActor = HitResult.GetActor();
		if (HitActor)
		{
			
		}
		
	}
	

	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 1.0f, 0, 1.0f);
}

FVector ARifle::ApplySprayPattern(FVector OriginalDirection)
{
	if (SprayPattern.Num() == 0) return OriginalDirection;

	FVector2D Pattern = SprayPattern[CurrentPatternIndex];
	CurrentPatternIndex = (CurrentPatternIndex + 1) % SprayPattern.Num();

	FRotator DirectionRotator = OriginalDirection.Rotation();
	DirectionRotator.Yaw += Pattern.X;
	DirectionRotator.Pitch += Pattern.Y;

	return DirectionRotator.Vector();
}

FVector ARifle::ApplyInaccuracy(FVector Direction, float Speed)
{
	if (Speed <= 0) return Direction;

	float InaccuracyAmount = FMath::Min(Speed / 1000, 1.0f) * MaxWalkingInaccuracy; 

	float InaccuracyYaw = FMath::RandRange(-InaccuracyAmount, InaccuracyAmount);
	float InaccuracyPitch = FMath::RandRange(-InaccuracyAmount, InaccuracyAmount);

	FRotator DirectionRotator = Direction.Rotation();
	DirectionRotator.Yaw += InaccuracyYaw;
	DirectionRotator.Pitch += InaccuracyPitch;

	return DirectionRotator.Vector();
}

