// Fill out your copyright notice in the Description page of Project Settings.


#include "SpectatorCharactor.h"

#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"


ASpectatorCharactor::ASpectatorCharactor()
{
	PrimaryActorTick.bCanEverTick = true;
}


void ASpectatorCharactor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASpectatorCharactor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpectatorCharactor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

