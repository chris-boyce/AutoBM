// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreboardWidget.h"

#include "Components/VerticalBox.h"
#include "Misc/FileHelper.h"
#include "ScoreboardGraphWidget.h"
#include "RatingWidgetGraph.h"
#include "HAL/PlatformFilemanager.h"


void UScoreboardWidget::Activate()
{
	ScoreCanvas->SetVisibility(ESlateVisibility::Visible);
	isStats = true;
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
			ScoreboardStats.NormalizedScore = FCString::Atof(*Columns[8]);
			ScoreboardArray.Add(ScoreboardStats);
		}
	}
	for(auto& Score : ScoreboardArray)
	{
		auto temp = CalculateRating(Score.NormalizedScore);
		Score.FinalRating = temp.FinalRating;
		Score.Percentile = temp.Percentile;
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
	if(SwitchPageButton)
	{
		SwitchPageButton->OnClicked.AddDynamic(this, &UScoreboardWidget::HandleSwitchPage);
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
	if(RatingWidget)
	{
		RatingWidget->RemoveFromViewport();
	}
	
	ScoreboardGraphWidgets.Empty();
	ScoreboardArray.Empty();
}

void UScoreboardWidget::HandleRefreshButton()
{
	Deactivate();
	Activate();
}

void UScoreboardWidget::CountRatingInterval()
{
	const int32 MinValue = -10;
	const int32 MaxValue = 10;
	const int32 IntervalCount = MaxValue - MinValue;

	TArray<int32> Counts;
	Counts.Init(0, IntervalCount);

	for (auto Value : ScoreboardArray)
	{
		if (Value.FinalRating >= MinValue && Value.FinalRating <= MaxValue)
		{
			int32 Index = FMath::FloorToInt(Value.FinalRating) - MinValue;
			Counts[Index]++;
		}
	}
	
	for (int32 i = 0; i < Counts.Num(); ++i)
	{
		float IntervalStart = i + MinValue;
		float IntervalEnd = IntervalStart + 1.0f;
		UE_LOG(LogTemp, Warning, TEXT("Interval [%f, %f): %d"), IntervalStart, IntervalEnd, Counts[i]);
	}
	RatingWidget->CreateGraph(Counts);
	
	
}

void UScoreboardWidget::DisplayNextRatingGraph()
{
	if (CurrentScoreboardIndex < ScoreboardArray.Num())
	{
		const auto& ScoreboardItem = ScoreboardArray[CurrentScoreboardIndex];
		URatingWidgetGraph* RatingGraphWidget = CreateWidget<URatingWidgetGraph>(GetWorld(), RatingGraphClass);
		if (RatingGraphWidget)
		{
			RatingGraphWidget->UpdateWidgetText(ScoreboardItem);
			if (RatingGraph)
			{
				RatingGraph->AddChild(RatingGraphWidget);
			}
		}

		CurrentScoreboardIndex++;
		GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
		{
			DisplayNextRatingGraph();
		});
	}
	else
	{
		CountRatingInterval();
	}
}

void UScoreboardWidget::DisplayRatingGraph()
{
	CurrentScoreboardIndex = 0;
	SortArray(ESortOptions::Rating);
	DisplayNextRatingGraph();
}

void UScoreboardWidget::HandleSwitchPage()
{
	if(isStats)
	{
		ScoreCanvas->SetVisibility(ESlateVisibility::Hidden);
		RatingWidget = CreateWidget<URatingWidget>(GetWorld(), RatingWidgetClass);
		RatingWidget->AddToViewport();
		RatingWidget->SwitchPageButton->OnClicked.AddDynamic(this, &UScoreboardWidget::HandleSwitchPage);
		RatingGraph = RatingWidget->RatingGraphScrollBox;
		DisplayRatingGraph();
		isStats = false;
	}
	else
	{
		ScoreCanvas->SetVisibility(ESlateVisibility::Visible);
		RatingWidget->RemoveFromViewport();
		isStats = true;
	}
	
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

FRatingStruct UScoreboardWidget::CalculateRating(float NormalizedScore)
{
	float MeanTotal = 0;
	for(auto Score : ScoreboardArray)
	{
		MeanTotal += Score.NormalizedScore;
	}
	float Mean = MeanTotal / static_cast<float>(ScoreboardArray.Num());
	
	float Variance = 0;
	for (auto Score : ScoreboardArray)
	{
		Variance += FMath::Pow(Score.NormalizedScore - Mean, 2);
	}
	float FinalVarience = Variance / static_cast<float>(ScoreboardArray.Num() - 1);

	float STDDev = FMath::Sqrt(FinalVarience);
	
	float ZScore = (NormalizedScore - Mean) / STDDev;
	
	float t = 1 / (1 + P * FMath::Abs(ZScore));
	float t_pow = t; 
	float b_sum = B1 * t_pow;
	t_pow *= t; 
	b_sum += B2 * t_pow;
	t_pow *= t; 
	b_sum += B3 * t_pow;
	t_pow *= t; 
	b_sum += B4 * t_pow;
	t_pow *= t; 
	b_sum += B5 * t_pow;
	
	float cdf = 1 - (1 / (FMath::Sqrt(2 * PI))) * FMath::Exp(-FMath::Pow(ZScore, 2) / 2) * b_sum;
	float percentile = (ZScore < 0) ? 1 - cdf : cdf;
	
	FRatingStruct temp;
	
	float FinalRating = FMath::Lerp(-10.0f, 10.0f, percentile);
	temp.FinalRating = FinalRating;
	temp.Percentile = percentile;
	
	return temp;
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
	case ESortOptions::Rating:
		ScoreboardArray.Sort([](const FScoreboardStats& A, const FScoreboardStats& B)
		{
			return A.FinalRating > B.FinalRating;
		});
	default: ;
	}
}


