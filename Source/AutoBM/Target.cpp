// Fill out your copyright notice in the Description page of Project Settings.


#include "Target.h"

#include "AIController.h"
#include "AITracker.h"
#include "HUDManager.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NavigationSystem.h"
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

	

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	AIRifle = GetWorld()->SpawnActor<AAIRifle>(AIRifleClass, GetActorLocation(), GetActorRotation(), SpawnParams);
	AIRifle->AttachToComponent(SkeletalMeshComponent, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("GunSocket"));

	HeadDirectionsComp = AIRifle->GetComponentByClass<UBoxComponent>();

	AAIController* AIController = Cast<AAIController>(GetController());

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
		TakeDamage(DamageInfo.HeadshotDamage);
	}
	else if(HitComponent == BodyCollision)
	{
		TakeDamage(DamageInfo.BodyShotDamage);
	}
	else
	{
		TakeDamage(DamageInfo.AppendageDamage);
	}
	
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
	OnDeath.Broadcast();
	AIRifle->Destroy();
	DetachFromControllerPendingDestroy();
	

	HeadCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BodyCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftLegCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightLegCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CapsuleComponentz->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	GetCharacterMovement()->DisableMovement();
	SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	SkeletalMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	SkeletalMeshComponent->SetSimulatePhysics(true);

	

	
	
	FTimerHandle RagdollTimerHandle;
	GetWorldTimerManager().SetTimer(RagdollTimerHandle, this, &ATarget::DestroyPawn, 5.0f, false);

	
}

void ATarget::DestroyPawn()
{
	Destroy();
}

AAIPath* ATarget::GetAIPath()
{
	return AIPath;
}

FVector ATarget::GetHeadLocation()
{
	return HeadCollision->GetComponentLocation();
}

FVector ATarget::GetBodyLocation()
{
	return BodyCollision->GetComponentLocation();
}






