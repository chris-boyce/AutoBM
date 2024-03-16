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
	RightLegCollision = Cast<UCapsuleComponent>(GetCapsuleByName("RightLegCapsule"));
	LeftLegCollision = Cast<UCapsuleComponent>(GetCapsuleByName("LeftLegCapsule"));
	if(HeadCollision && BodyCollision && RightLegCollision)
	{
		//HeadCollision->SetupAttachment(SkeletalMeshComponent, FName("headSocket"));
		//BodyCollision->SetupAttachment(SkeletalMeshComponent, FName("bodySocket"));
		//RightLegCollision->SetupAttachment(SkeletalMeshComponent, FName("rightLegSocket"));
	}
	
}

void ATarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
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
	if(HitComponent == HeadCollision)
	{
		UE_LOG(LogTemp, Warning, TEXT("Headshot"));
	}
	else if(HitComponent == BodyCollision)
	{
		UE_LOG(LogTemp, Warning, TEXT("Bodyshot"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Else Shot"));
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Hit Component Name: %s"), *ComponentName)
}




