// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDManager.h"

#include "Target.h"


AHUDManager::AHUDManager()
{
}

void AHUDManager::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* Player = GetWorld()->GetFirstPlayerController();
	PlayerController = Cast<AFirstPersonCharacterController>(Player);
	AddKillFeedHUD();
	
}



void AHUDManager::AddKillFeedHUD()
{
	KillFeedWidget = CreateWidget<UKillFeedWidget>(GetWorld(), KillFeedClass);
	if(KillFeedWidget)
	{
		KillFeedWidget->AddToViewport();
	}
	
	
}

void AHUDManager::AddEnemyToSubList(ATarget* Enemy)
{
	UE_LOG(LogTemp, Warning, TEXT("HAS SUBBED"));
	Enemy->OnDeath.AddDynamic(KillFeedWidget, &UKillFeedWidget::EnemyDeath);

}

