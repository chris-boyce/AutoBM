// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shootable.h"
#include "GameFramework/Character.h"
#include "Target.generated.h"

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

	UActorComponent* GetCapsuleByName(FName CompName);

	virtual void HandleHit(UPrimitiveComponent* HitComponent, FDamageInfo DamageInfo) override;

	UPROPERTY(EditAnywhere, Category="Debug")
	bool DrawDebug = false;
};
