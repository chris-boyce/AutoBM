// Fill out your copyright notice in the Description page of Project Settings.


#include "RatingWidgetGraph.h"

#include "ScoreboardWidget.h"


void URatingWidgetGraph::UpdateWidgetText(FScoreboardStats Score)
{
	TB0->SetText(FText::FromString(FString(TEXT("Bot")) + Score.Name));
	TB1->SetText(FText::AsNumber(Score.FinalRating));
	TB2->SetText(FText::AsNumber(Score.Percentile));
}


