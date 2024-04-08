// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Target.h"
#include "Camera/CameraActor.h"
#include "AutoCam.generated.h"


UCLASS()
class AUTOBM_API AAutoCam : public ACameraActor
{
	GENERATED_BODY()

	void CheckBotVisibility();
	
	virtual void BeginPlay() override;

	UPROPERTY()
	APlayerController* Player;

	FTimerHandle TimerHandle;

	bool bIsBotVisible = false;

	bool bHasBeenUsed = false;

public:
	UPROPERTY(EditAnywhere)
	ATarget* TargetBot;



	UFUNCTION()
	void SetTargetBot(ATarget* Bot);

	
};
