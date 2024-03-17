// Fill out your copyright notice in the Description page of Project Settings.


#include "Target.h"

#include "HUDManager.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

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
	CapsuleComponentz = GetComponentByClass<UCapsuleComponent>();

	AActor* HUDManagerActor = UGameplayStatics::GetActorOfClass(GetWorld(), AHUDManager::StaticClass());
	AHUDManager* HUDManager = Cast<AHUDManager>(HUDManagerActor);
	HUDManager->AddEnemyToSubList(this);
	
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
		TakeDamage(DamageInfo.HeadshotDamage);
	}
	else if(HitComponent == BodyCollision)
	{
		UE_LOG(LogTemp, Warning, TEXT("Bodyshot"));
		TakeDamage(DamageInfo.BodyShotDamage);
		
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Else Shot"));
		TakeDamage(DamageInfo.AppendageDamage);
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Hit Component Name: %s"), *ComponentName)
}

void ATarget::TakeDamage(float Damage)
{
	CurrentHealth = CurrentHealth - Damage;
	if(CurrentHealth <= 0)
	{
		Death();
	}
}

void ATarget::Death()
{
	UE_LOG(LogTemp, Warning, TEXT("Death Called"));

	HeadCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BodyCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftLegCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightLegCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CapsuleComponentz->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	GetCharacterMovement()->DisableMovement();
	SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	SkeletalMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	SkeletalMeshComponent->SetSimulatePhysics(true);

	OnDeath.Broadcast();
	
	FTimerHandle RagdollTimerHandle;
	GetWorldTimerManager().SetTimer(RagdollTimerHandle, this, &ATarget::DestroyPawn, 5.0f, false);

	
}

void ATarget::DestroyPawn()
{
	Destroy();
}






