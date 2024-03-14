// Fill out your copyright notice in the Description page of Project Settings.


#include "Rifle.h"

#include "Components/DecalComponent.h"
#include "FirstPersonPlayer.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

ARifle::ARifle()
{
	PrimaryActorTick.bCanEverTick = true;
	SprayPattern = {
		FVector2D(0, 0),     // Start moving up
		FVector2D(0, 2.2),
		FVector2D(0.3, 4.5), // Start moving right
		FVector2D(0.6, 5),
		FVector2D(0.9, 6),   // More to the right
		FVector2D(-0.5, 7),  // Sharp left
		FVector2D(-0.7, 8),
		FVector2D(-0.4, 9),// Start tapering off left
		FVector2D(-0.2, 10),
		FVector2D(0.1, 11), // Slight right
		FVector2D(0.2, 12),
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
	CurrentPatternIndex = 0;
}

void ARifle::FireWeapon()
{
	WeaponFired.Broadcast();
	UE_LOG(LogTemp, Warning, TEXT("THIS IS RUNNING"));
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
		SpawnDecalAtLocation(HitResult.ImpactPoint, HitResult.ImpactNormal);
		UE_LOG(LogTemp, Warning, TEXT("HAS HIT AN OBJECT"));
		AActor* HitActor = HitResult.GetActor();
		if (HitActor)
		{
			
		}
		
	}
	

	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 1.0f, 0, 1.0f);
}

FVector ARifle::ApplySprayPattern(FVector OriginalDirection)
{
	if(CurrentPatternIndex > 10)
	{
		return OriginalDirection;
	}
	FVector2D Pattern = SprayPattern[CurrentPatternIndex];
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
	
	float ClampedSpeed = FMath::Clamp(PlayerSpeed, 0.0f, 10.0f);
	
	float InaccuracyAmount = ClampedSpeed;

	float InaccuracyYaw = FMath::RandRange(-InaccuracyAmount, InaccuracyAmount);
	float InaccuracyPitch = FMath::RandRange(-InaccuracyAmount, InaccuracyAmount);

	FRotator DirectionRotator = Direction.Rotation();
	DirectionRotator.Yaw += InaccuracyYaw;
	DirectionRotator.Pitch += InaccuracyPitch;

	FVector NewDirection = DirectionRotator.Vector();
	
	//UE_LOG(LogTemp, Warning, TEXT("New Direction Vector: X=%f, Y=%f, Z=%f"), NewDirection.X, NewDirection.Y, NewDirection.Z);

	return NewDirection;
}

void ARifle::SpawnDecalAtLocation(FVector& Location, FVector& Normal)
{
	if (BulletDecalMaterial)
	{
		UDecalComponent* Decal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), BulletDecalMaterial, FVector(10.0f, 10.0f, 10.0f), Location, Normal.Rotation());
		// Set fade start and end parameters to control the visibility distance of the decal
		Decal->SetFadeScreenSize(0.001f); // Start fading when the decal size reaches 1% of the screen size
		UE_LOG(LogTemp, Warning, TEXT("DEcal"));
		
	}
}

