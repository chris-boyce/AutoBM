// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIPath.h"
#include "AIRifle.h"
#include "Shootable.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Target.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

class USphereComponent;
UCLASS()
class AUTOBM_API ATarget : public ACharacter, public IShootable
{
	GENERATED_BODY()

public:
	ATarget();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category="BodySetup")
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(EditAnywhere, Category="BodySetup")
	USphereComponent* HeadCollision;

	UPROPERTY(EditAnywhere, Category="BodySetup")
	UCapsuleComponent* BodyCollision;

	UPROPERTY(EditAnywhere, Category="BodySetup")
	UCapsuleComponent* RightLegCollision;

	UPROPERTY(EditAnywhere, Category="BodySetup")
	UCapsuleComponent* LeftLegCollision;

	UPROPERTY(EditAnywhere, Category="BodySetup")
	UCapsuleComponent* CapsuleComponentz;

	UPROPERTY(EditAnywhere, Category="BodySetup")
	UBoxComponent* HeadDirectionsComp;

	UActorComponent* GetCapsuleByName(FName CompName);

	virtual void HandleHit(UPrimitiveComponent* HitComponent, FDamageInfo DamageInfo) override;

	UPROPERTY(VisibleAnywhere, Category="Health")
	float MaxHealth = 100;
	
	UPROPERTY(VisibleAnywhere, Category="Health")
	float CurrentHealth = MaxHealth;

	void TakeDamage(float Damage);

	void Death();

	void DestroyPawn();

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="AI")
	AAIPath* AIPath;

	UFUNCTION()
	AAIPath* GetAIPath();
	
	UPROPERTY()
	FOnDeath OnDeath;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AAIRifle> AIRifleClass;

	UPROPERTY()
	AAIRifle* AIRifle;

	
};
