// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstPersonWidget.h"

void UFirstPersonWidget::UpdateAmmo(int CurrentAmmoNum, int FullAmmoAmount)
{
	UE_LOG(LogTemp, Warning, TEXT("Called HUD"));
	FString CurrentAmmoString = FString::FromInt(CurrentAmmoNum);
	FString FullAmmoString = FString::FromInt(FullAmmoAmount);
	
	CurrentAmmo->SetText(FText::FromString(CurrentAmmoString));
	FullAmmo->SetText(FText::FromString(FullAmmoString));
}

void UFirstPersonWidget::UpdateReloadBar(float ReloadTime)
{
	ReloadBar->SetVisibility(ESlateVisibility::Visible);
	m_ReloadTime = ReloadTime;
	if (ReloadTime > 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
		
		ReloadProgress = 0.0f;
		
		int32 TotalUpdates = FMath::CeilToInt(ReloadTime / 0.1f);
		
		IncrementValue = 0.5f / TotalUpdates;
		
		GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &UFirstPersonWidget::UpdateProgressBar, 0.05f, true, 0.0f);
	}
}

void UFirstPersonWidget::UpdateProgressBar()
{
	ReloadProgress += IncrementValue;
	
	ReloadProgress = FMath::Clamp(ReloadProgress, 0.0f, m_ReloadTime);
	
	SetReloadProgressBar(ReloadProgress);
	
	if (ReloadProgress >= 1.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
		ReloadBar->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UFirstPersonWidget::SetReloadProgressBar(float Value)
{
	if (ReloadBar)
	{
		ReloadBar->SetPercent(Value);
	}
}
