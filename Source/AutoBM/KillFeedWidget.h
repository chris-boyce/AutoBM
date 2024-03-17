// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KillFeedWidget.generated.h"

USTRUCT(BlueprintType)
struct FKillFeedData
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FString Killer;

	UPROPERTY()
	FString Weapon;

	UPROPERTY()
	FString Killed;
};

UCLASS()
class AUTOBM_API UKillFeedWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void EnemyDeath();

	UPROPERTY()
	TArray<FKillFeedData> KillFeed;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    class UVerticalBox* KillFeedVerticalBox;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> KillFeedGraphClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UWidgetAnimation> GraphAnimation;

	
};
