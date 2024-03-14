// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tracer.generated.h"

UCLASS()
class AUTOBM_API ATracer : public AActor
{
	GENERATED_BODY()
	
public:	
	ATracer();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	void StartMovement(const FVector& Direction);
private:
	FTimerHandle MovementTimerHandle;

	FVector ProjectileDirection;

};
