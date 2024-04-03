// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDManager.h"
#include "Target.h"


AHUDManager::AHUDManager()
{
}

void AHUDManager::BeginPlay()
{
	AddKillFeedHUD();
	APlayerController* Player = GetWorld()->GetFirstPlayerController();
	PlayerController = Cast<AFirstPersonCharacterController>(Player);
	ScoreboardWidget = CreateWidget<UScoreboardWidget>(GetWorld(), ScoreboardClass);
	ScoreboardWidget->AddToViewport();
	ScoreboardWidget->SetVisibility(ESlateVisibility::Hidden);
	KillFeedWidget = CreateWidget<UKillFeedWidget>(GetWorld(), KillFeedClass);
	Super::BeginPlay();
	
}



void AHUDManager::AddKillFeedHUD()
{
	
	if(KillFeedWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("Has Made HUD"));
		KillFeedWidget->AddToViewport();
	}
	
	
}

void AHUDManager::AddEnemyToSubList(ATarget* Enemy)
{
	if(KillFeedWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("HAS SUBBED"));
		Enemy->OnDeath.AddDynamic(KillFeedWidget, &UKillFeedWidget::EnemyDeath);
	}
}

void AHUDManager::ToggleScoreboardWidget()
{
	if(IsScoreboardVisible)
	{
		ScoreboardWidget->SetVisibility(ESlateVisibility::Hidden);
		ScoreboardWidget->Deactivate();
		IsScoreboardVisible = false;
	}
	else
	{
		ScoreboardWidget->SetVisibility(ESlateVisibility::Visible);
		ScoreboardWidget->Activate();
		IsScoreboardVisible = true;
	}
}

