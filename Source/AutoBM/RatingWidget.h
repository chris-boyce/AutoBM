// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "RatingWidget.generated.h"


UCLASS()
class AUTOBM_API URatingWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, Category = "HUD", meta = (BindWidget))
	UButton* SwitchPageButton;

	UPROPERTY(BlueprintReadOnly, Category = "HUD", meta = (BindWidget))
	UScrollBox* RatingGraphScrollBox;

	UPROPERTY(BlueprintReadOnly, Category = "HUD", meta = (BindWidget))
	UHorizontalBox* GraphBox;

	UFUNCTION()
	void CreateGraph(TArray<int32> Counts);

	UFUNCTION()
	void GetAllImages();

	UPROPERTY()
	TArray<UImage*> Lines;
	
};
