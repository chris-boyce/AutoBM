// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreboardGraphWidget.h"

#include "ScoreboardWidget.h"
#include "Components/TextBlock.h"

void UScoreboardGraphWidget::UpdateWidgetText(FScoreboardStats ScoreboardStats)
{
	TB0->SetText(FText::FromString(FString(TEXT("Bot")) + ScoreboardStats.Name));
	TB1->SetText(FText::AsNumber(ScoreboardStats.Accuracy));
	TB2->SetText(FText::AsNumber(ScoreboardStats.HeadshotPercentage));
	TB3->SetText(FText::AsNumber(ScoreboardStats.TimeToDamage));
	TB4->SetText(FText::AsNumber(ScoreboardStats.TimeToKill));
	TB5->SetText(FText::AsNumber(ScoreboardStats.HitToDeath));
	TB6->SetText(FText::AsNumber(ScoreboardStats.MoveFiringPercentage));
	TB7->SetText(FText::AsNumber(ScoreboardStats.NormalizedScore));
}
