// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "ScoreboardWidget.generated.h"

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
	MoveFiringPercentage
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
	UButton* RefreshButton;

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

	
	UFUNCTION()
	void Deactivate();

	UFUNCTION()
	void HandleRefreshButton();

	UFUNCTION()
	void DisplayScoreboardGraph();

	UFUNCTION()
	void DisplayNextScoreboardGraph();

	UPROPERTY()
	int CurrentScoreboardIndex = 0;

	FTimerHandle ScoreboardItemTimerHandle;

	UFUNCTION()
	void SortArray(ESortOptions Option);

	ESortOptions SortOption;

	
	
};
