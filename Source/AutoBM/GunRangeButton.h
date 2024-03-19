// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "GunRangeButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FButtonPress, float, data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FButtonStart);

UCLASS()
class AUTOBM_API AGunRangeButton : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	AGunRangeButton();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Interact() override;

	FButtonPress ButtonPress;

	FButtonStart ButtonStart;

	UPROPERTY(EditAnywhere)
	bool bIsStartButton = false;
	
	UPROPERTY(EditAnywhere)
	float Speed = 3.0f;

};
