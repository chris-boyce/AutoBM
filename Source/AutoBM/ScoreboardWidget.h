// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RatingWidget.h"

#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/ScrollBox.h"
#include "ScoreboardWidget.generated.h"

class URatingWidgetGraph;
class UScoreboardGraphWidget;
USTRUCT(BlueprintType)
struct FScoreboardStats
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere)
	FString Name;
	UPROPERTY(VisibleAnywhere)
	float Accuracy;
	UPROPERTY(VisibleAnywhere)
	float HeadshotPercentage;
	UPROPERTY(VisibleAnywhere)
	float TimeToKill;
	UPROPERTY(VisibleAnywhere)
	float TimeToDamage;
	UPROPERTY(VisibleAnywhere)
	float HitToDeath;
	UPROPERTY(VisibleAnywhere)
	float MoveFiringPercentage;
	UPROPERTY(VisibleAnywhere)
	FString DidFinish;
	UPROPERTY(VisibleAnywhere)
	float NormalizedScore;
	UPROPERTY(VisibleAnywhere)
	float FinalRating;
	UPROPERTY(VisibleAnywhere)
	float Percentile;
};

USTRUCT(BlueprintType)
struct FRatingStruct
{
	GENERATED_BODY()
public:
	UPROPERTY()
	float FinalRating;
	UPROPERTY()
	float Percentile;
};

UENUM(BlueprintType)
enum class ESortOptions : uint8
{
	None,
	Name,
	Accuracy,
	HeadshotPercentage,
	TimeToKill,
	TimeToDamage,
	HitToDeath,
	MoveFiringPercentage,
	Rating
};


UCLASS()
class AUTOBM_API UScoreboardWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void Activate();
	
	virtual void NativeConstruct() override;

	UPROPERTY(VisibleAnywhere)
	TArray<FScoreboardStats> ScoreboardArray;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> ScoreboardGraph;

	UPROPERTY(VisibleAnywhere)
	TArray<UScoreboardGraphWidget*> ScoreboardGraphWidgets;

	UPROPERTY(BlueprintReadOnly, Category = "HUD", meta = (BindWidget))
	UScrollBox* GraphScrollBox;

	UPROPERTY(BlueprintReadOnly, Category = "HUD", meta = (BindWidget))
	UCanvasPanel* ScoreCanvas;

	UPROPERTY(EditAnywhere)
	TSubclassOf<URatingWidget> RatingWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<URatingWidgetGraph> RatingGraphClass;
	
	UPROPERTY()
	URatingWidget* RatingWidget;

	UPROPERTY(BlueprintReadOnly, Category = "HUD", meta = (BindWidget))
	UButton* RefreshButton;

	UPROPERTY(BlueprintReadOnly, Category = "HUD", meta = (BindWidget))
	UButton* SwitchPageButton;

	UPROPERTY(BlueprintReadOnly, Category = "HUD", meta = (BindWidget))
	UButton* SortNone;
	UFUNCTION()
	void HandleSortNone(){SortOption = ESortOptions::None ;HandleRefreshButton();};
	
	UPROPERTY(BlueprintReadOnly, Category = "HUD", meta = (BindWidget))
	UButton* SortName;
	UFUNCTION()
	void HandleSortName(){SortOption = ESortOptions::Name ; HandleRefreshButton();}
	
	UPROPERTY(BlueprintReadOnly, Category = "HUD", meta = (BindWidget))
	UButton* SortHSP;
	UFUNCTION()
	void HandleSortHSP(){SortOption = ESortOptions::HeadshotPercentage ;HandleRefreshButton();}
	
	UPROPERTY(BlueprintReadOnly, Category = "HUD", meta = (BindWidget))
	UButton* SortTTK;
	UFUNCTION()
	void HandleSortTTK(){SortOption = ESortOptions::TimeToKill ;HandleRefreshButton();}
	
	UPROPERTY(BlueprintReadOnly, Category = "HUD", meta = (BindWidget))
	UButton* SortTTD;
	UFUNCTION()
	void HandleSortTTD(){SortOption = ESortOptions::TimeToDamage ;HandleRefreshButton();}
	
	UPROPERTY(BlueprintReadOnly, Category = "HUD", meta = (BindWidget))
	UButton* SortHTD;
	UFUNCTION()
	void HandleSortHTD(){SortOption = ESortOptions::HitToDeath ;HandleRefreshButton();}
	
	UPROPERTY(BlueprintReadOnly, Category = "HUD", meta = (BindWidget))
	UButton* SortAcc;
	UFUNCTION()
	void HandleSortAcc(){SortOption = ESortOptions::Accuracy ;HandleRefreshButton();}
	
	UPROPERTY(BlueprintReadOnly, Category = "HUD", meta = (BindWidget))
	UButton* SortMFP;
	UFUNCTION()
	void HandleSortMFP(){SortOption = ESortOptions::MoveFiringPercentage ;HandleRefreshButton();}

	UScrollBox* RatingGraph;
	
	UFUNCTION()
	void Deactivate();

	UFUNCTION()
	void HandleRefreshButton();

	UFUNCTION()
	void CountRatingInterval();

	void DisplayNextRatingGraph();
	UFUNCTION()
	void DisplayRatingGraph();
	
	UFUNCTION()
	void HandleSwitchPage();

	UFUNCTION()
	void DisplayScoreboardGraph();

	UFUNCTION()
	void DisplayNextScoreboardGraph();

	UPROPERTY()
	int CurrentScoreboardIndex = 0;

	UFUNCTION()
	FRatingStruct CalculateRating(float NormalizedScore);

	

	FTimerHandle ScoreboardItemTimerHandle;

	UFUNCTION()
	void SortArray(ESortOptions Option);

	ESortOptions SortOption;

	UPROPERTY(EditAnywhere)
	UCurveFloat* Curve;

	float P = 0.2316419;
	float B1 = 0.319381530;
	float B2 = -0.356563782;
	float B3 = 1.781477937;
	float B4 = -1.821255978;
	float B5 = 1.330274429;

	bool isStats = true;
};
