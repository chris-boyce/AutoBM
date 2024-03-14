// Fill out your copyright notice in the Description page of Project Settings.


#include "Tracer.h"

// Sets default values
ATracer::ATracer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetLifeSpan(1.0f);
}

// Called when the game starts or when spawned
void ATracer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATracer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!ProjectileDirection.IsNearlyZero())
	{
		FVector NewLocation = GetActorLocation() + ProjectileDirection * 5000.0f * DeltaTime;
		SetActorLocation(NewLocation, true);
	}
}

void ATracer::StartMovement(const FVector& Direction)
{
	ProjectileDirection = Direction.GetSafeNormal();
	
}

