// Fill out your copyright notice in the Description page of Project Settings.


#include "KillFeedWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/VerticalBox.h"

void UKillFeedWidget::EnemyDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("Kill Feed Widget Called"));
	UUserWidget* KillFeedGraphWidget = CreateWidget<UUserWidget>(this, KillFeedGraphClass);
	KillFeedVerticalBox->AddChild(KillFeedGraphWidget);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, KillFeedGraphWidget]()
	{
		KillFeedVerticalBox->RemoveChild(KillFeedGraphWidget);
		KillFeedGraphWidget->RemoveFromParent();
	}, 3.0f, false);
}
