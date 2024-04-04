// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "RatingWidgetGraph.generated.h"

struct FScoreboardStats;

UCLASS()
class AUTOBM_API URatingWidgetGraph : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void UpdateWidgetText(FScoreboardStats Score);

	UPROPERTY(BlueprintReadOnly, Category = "TextBoxs", meta = (BindWidget))
	UTextBlock* TB0 = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "TextBoxs", meta = (BindWidget))
	UTextBlock* TB1 = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "TextBoxs", meta = (BindWidget))
	UTextBlock* TB2 = nullptr;
	
};
