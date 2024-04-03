// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreboardWidget.h"

#include "Components/VerticalBox.h"
#include "Misc/FileHelper.h"
#include "ScoreboardGraphWidget.h"
#include "HAL/PlatformFilemanager.h"


void UScoreboardWidget::Activate()
{
	TArray<FString> Lines;

	FString FilePath = FPaths::ProjectSavedDir() + TEXT("BotData.csv");
	FFileHelper::LoadFileToStringArray(Lines, *FilePath);
	
	for (FString& Line : Lines)
	{
		TArray<FString> Columns;
		Line.ParseIntoArray(Columns, TEXT(","), true);
		if (Columns.Num() >= 8) 
		{
			FScoreboardStats ScoreboardStats;
			ScoreboardStats.Name = Columns[0];
			ScoreboardStats.Accuracy = FCString::Atof(*Columns[1]);
			ScoreboardStats.HeadshotPercentage = FCString::Atof(*Columns[2]);
			ScoreboardStats.TimeToKill = FCString::Atof(*Columns[3]);
			ScoreboardStats.TimeToDamage = FCString::Atof(*Columns[4]);
			ScoreboardStats.HitToDeath = FCString::Atof(*Columns[5]);
			ScoreboardStats.MoveFiringPercentage = FCString::Atof(*Columns[6]);
			ScoreboardStats.DidFinish = Columns[7];
			
			ScoreboardArray.Add(ScoreboardStats);
		}
	}
	SortArray(SortOption);
	DisplayScoreboardGraph();
}

void UScoreboardWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (RefreshButton)
	{
		RefreshButton->OnClicked.AddDynamic(this, &UScoreboardWidget::HandleRefreshButton);
	}
	SortNone->OnClicked.AddDynamic(this, &UScoreboardWidget::HandleSortNone);
	SortName->OnClicked.AddDynamic(this, &UScoreboardWidget::HandleSortName);
	SortHSP->OnClicked.AddDynamic(this, &UScoreboardWidget::HandleSortHSP);
	SortTTK->OnClicked.AddDynamic(this, &UScoreboardWidget::HandleSortTTK);
	SortTTD->OnClicked.AddDynamic(this, &UScoreboardWidget::HandleSortTTD);
	SortHTD->OnClicked.AddDynamic(this, &UScoreboardWidget::HandleSortHTD);
	SortAcc->OnClicked.AddDynamic(this, &UScoreboardWidget::HandleSortAcc);
	SortMFP->OnClicked.AddDynamic(this, &UScoreboardWidget::HandleSortMFP);
	
	SortOption = ESortOptions::None;
}

void UScoreboardWidget::Deactivate()
{
	for (UScoreboardGraphWidget* Widget : ScoreboardGraphWidgets)
	{
		if (Widget && Widget->GetParent())
		{
			Widget->RemoveFromParent();
		}
	}
	
	ScoreboardGraphWidgets.Empty();
	ScoreboardArray.Empty();
}

void UScoreboardWidget::HandleRefreshButton()
{
	Deactivate();
	Activate();
}

void UScoreboardWidget::DisplayScoreboardGraph()
{
	CurrentScoreboardIndex = 0;
	DisplayNextScoreboardGraph();
}

void UScoreboardWidget::DisplayNextScoreboardGraph()
{
	if (CurrentScoreboardIndex < ScoreboardArray.Num())
	{
		const auto& ScoreboardItem = ScoreboardArray[CurrentScoreboardIndex];
		UE_LOG(LogTemp, Warning, TEXT("Score Boarded"));
		UScoreboardGraphWidget* ScoreboardWidget = CreateWidget<UScoreboardGraphWidget>(GetWorld(), ScoreboardGraph);
		if (ScoreboardWidget)
		{
			ScoreboardWidget->UpdateWidgetText(ScoreboardItem);
			ScoreboardGraphWidgets.Add(ScoreboardWidget);

			if (GraphScrollBox)
			{
				GraphScrollBox->AddChild(ScoreboardWidget);
			}
		}

		CurrentScoreboardIndex++;
		GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
		{
			DisplayNextScoreboardGraph();
		});
	}
}

void UScoreboardWidget::SortArray(ESortOptions Option)
{
	switch (Option)
	{
	case ESortOptions::Name:
		ScoreboardArray.Sort([](const FScoreboardStats& A, const FScoreboardStats& B)
			{
				return A.Name < B.Name;
			});
		break;
	case ESortOptions::Accuracy:
		ScoreboardArray.Sort([](const FScoreboardStats& A, const FScoreboardStats& B)
			{
				return A.Accuracy > B.Accuracy;
			});
		break;
	case ESortOptions::HeadshotPercentage:
		ScoreboardArray.Sort([](const FScoreboardStats& A, const FScoreboardStats& B)
			{
				return A.HeadshotPercentage > B.HeadshotPercentage;
			});
		break;
	case ESortOptions::TimeToKill:
		ScoreboardArray.Sort([](const FScoreboardStats& A, const FScoreboardStats& B)
			{
				return A.TimeToKill < B.TimeToKill;
			});
		break;
	case ESortOptions::TimeToDamage:
		ScoreboardArray.Sort([](const FScoreboardStats& A, const FScoreboardStats& B)
			{
				return A.TimeToDamage < B.TimeToDamage;
			});
		break;
	case ESortOptions::HitToDeath:
		ScoreboardArray.Sort([](const FScoreboardStats& A, const FScoreboardStats& B)
				{
					return A.HitToDeath < B.HitToDeath;
				});
		break;
	case ESortOptions::MoveFiringPercentage:
		ScoreboardArray.Sort([](const FScoreboardStats& A, const FScoreboardStats& B)
				{
					return A.MoveFiringPercentage < B.MoveFiringPercentage;
				});
		break;
	default: ;
	}
}


