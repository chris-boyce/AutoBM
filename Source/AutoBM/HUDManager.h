// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FirstPersonCharacterController.h"
#include "FirstPersonPlayer.h"
#include "KillFeedWidget.h"
#include "GameFramework/Actor.h"
#include "HUDManager.generated.h"
class ATarget;
UCLASS()
class AUTOBM_API AHUDManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AHUDManager();

protected:
	virtual void BeginPlay() override;

public:	
	
	UPROPERTY()
	AFirstPersonCharacterController* PlayerController;

	void AddKillFeedHUD();

	UPROPERTY(EditAnywhere, Category="KillFeed")
	TSubclassOf<UUserWidget> KillFeedClass;

	UPROPERTY(EditAnywhere, Category="KillFeed")
	UKillFeedWidget* KillFeedWidget;

	
	UFUNCTION()
	void AddEnemyToSubList(ATarget* Enemy);

	
	

};

