// Fill out your copyright notice in the Description page of Project Settings.


#include "AutoCam.h"

#include "Kismet/GameplayStatics.h"


void AAutoCam::BeginPlay()
{
	Player = GetWorld()->GetFirstPlayerController();
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AAutoCam::CheckBotVisibility, 0.25f, true);
	
}

void AAutoCam::CheckBotVisibility()
{
	if(!TargetBot || bHasBeenUsed)
	{
		return;
	}
	FVector StartLocation = GetActorLocation();
	FVector EndLocation = TargetBot->GetActorLocation();
	FHitResult HitResult;
	bIsBotVisible = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility);
	if (bIsBotVisible && HitResult.GetActor() == TargetBot)
	{
		Player->SetViewTarget(this);
		bHasBeenUsed = true;
	}
}

void AAutoCam::SetTargetBot(ATarget* Bot)
{
	UE_LOG(LogTemp, Warning, TEXT("Bot Has Been Set"));
	TargetBot = Bot;
	bHasBeenUsed = false;
}
