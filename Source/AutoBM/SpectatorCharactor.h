// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpectatorCharactor.generated.h"

UCLASS()
class AUTOBM_API ASpectatorCharactor : public ACharacter
{
	GENERATED_BODY()

public:

	ASpectatorCharactor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
};
