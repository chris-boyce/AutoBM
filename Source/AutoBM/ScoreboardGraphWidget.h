// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreboardGraphWidget.generated.h"


struct FScoreboardStats;
class UTextBlock;
UCLASS()
class AUTOBM_API UScoreboardGraphWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, Category = "TextBoxs", meta = (BindWidget))
	UTextBlock* TB0 = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "TextBoxs", meta = (BindWidget))
	UTextBlock* TB1 = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "TextBoxs", meta = (BindWidget))
	UTextBlock* TB2 = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "TextBoxs", meta = (BindWidget))
	UTextBlock* TB3 = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "TextBoxs", meta = (BindWidget))
	UTextBlock* TB4 = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "TextBoxs", meta = (BindWidget))
	UTextBlock* TB5 = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "TextBoxs", meta = (BindWidget))
	UTextBlock* TB6 = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "TextBoxs", meta = (BindWidget))
	UTextBlock* TB7 = nullptr;

	UFUNCTION()
	void UpdateWidgetText(FScoreboardStats ScoreboardStats);
	
};
