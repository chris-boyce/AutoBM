// Fill out your copyright notice in the Description page of Project Settings.


#include "Target.h"

#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"

ATarget::ATarget()
{
	PrimaryActorTick.bCanEverTick = true;
	SkeletalMeshComponent = GetMesh();
	
	
	
}

void ATarget::BeginPlay()
{
	Super::BeginPlay();
	HeadCollision = FindComponentByClass<USphereComponent>();
	BodyCollision = Cast<UCapsuleComponent>(GetCapsuleByName("BodyCapsule"));
	if(HeadCollision && BodyCollision)
	{
		HeadCollision->SetupAttachment(SkeletalMeshComponent, FName("headSocket"));
		BodyCollision->SetupAttachment(SkeletalMeshComponent, FName("bodySocket"));
		UE_LOG(LogTemp, Warning, TEXT("THIS HAS WORKED"));
	}
	
}

void ATarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	HeadCollision->SetWorldLocation(SkeletalMeshComponent->GetSocketLocation(FName("headSocket")));
	BodyCollision->SetWorldLocation(SkeletalMeshComponent->GetSocketLocation(FName("bodySocket")));
	
	if(DrawDebug)
	{
		DrawDebugSphere(GetWorld(), HeadCollision->GetComponentLocation(), HeadCollision->GetScaledSphereRadius(), 32, FColor::Green, false, 0.1f, 0, 2);

	
		FVector CapsuleLocation = BodyCollision->GetComponentLocation();
		FRotator CapsuleRotation = BodyCollision->GetComponentRotation();

		FQuat CapsuleQuat = FQuat(CapsuleRotation);
	
		float CapsuleHalfHeight = BodyCollision->GetScaledCapsuleHalfHeight();
		float CapsuleRadius = BodyCollision->GetScaledCapsuleRadius();
	
		DrawDebugCapsule(GetWorld(), CapsuleLocation, CapsuleHalfHeight, CapsuleRadius, CapsuleQuat, FColor::Green, false, -1, 0, 2);
	}
	

}

void ATarget::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UActorComponent* ATarget::GetCapsuleByName(FName CompName)
{
	TArray<UActorComponent*> Components;
	GetComponents(Components);
	for (UActorComponent* Component : Components)
	{
		if (Component && Component->GetFName() == CompName)
		{
			return Component;
		}
	}
	return nullptr;
}

void ATarget::HandleHit(UPrimitiveComponent* HitComponent, FDamageInfo DamageInfo)
{
	FString ComponentName = HitComponent->GetName();
	UE_LOG(LogTemp, Warning, TEXT("ON THE TARGET"));
	UE_LOG(LogTemp, Warning, TEXT("Hit Component Name: %s"), *ComponentName)
}




