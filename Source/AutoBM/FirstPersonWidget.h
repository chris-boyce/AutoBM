// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "FirstPersonWidget.generated.h"

UCLASS()
class AUTOBM_API UFirstPersonWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void UpdateAmmo(int CurrentAmmoNum, int FullAmmoAmount);

	UFUNCTION()
	void UpdateReloadBar(float ReloadTime);

	void UpdateProgressBar();

	void SetReloadProgressBar(float Value);

	UPROPERTY(BlueprintReadOnly, Category = "Ammo", meta = (BindWidget))
	UTextBlock* CurrentAmmo = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Ammo", meta = (BindWidget))
	UTextBlock* FullAmmo = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Ammo", meta = (BindWidget))
	UProgressBar* ReloadBar;

	UPROPERTY()
	float ReloadProgress;
	UPROPERTY()
	float IncrementValue;

	float m_ReloadTime;

	FTimerHandle ReloadTimerHandle;
private:
	
	
};
