// Fill out your copyright notice in the Description page of Project Settings.


#include "RatingWidget.h"


void URatingWidget::CreateGraph(TArray<int32> Counts)
{
	GetAllImages();
	for (int32 i = 0; i < Lines.Num(); ++i)
	{
		FSlateBrush Brush = Lines[i]->Brush;
		
		Brush.ImageSize.Y = Counts[i] * 10.0f;

		Lines[i]->SetBrush(Brush);
	}
}

void URatingWidget::GetAllImages()
{
	for (int32 i = 0; i < GraphBox->GetChildrenCount(); ++i)
	{
		UWidget* ChildWidget = GraphBox->GetChildAt(i);
		
		if (UImage* ImageWidget = Cast<UImage>(ChildWidget))
		{
			Lines.Add(ImageWidget);
		}
	}
}
