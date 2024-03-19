// Fill out your copyright notice in the Description page of Project Settings.


#include "GunRangeController.h"

#include "GunRangeSpawnPoint.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"


AGunRangeController::AGunRangeController()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AGunRangeController::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGunRangeSpawnPoint::StaticClass(), (TArray<AActor*>&)SpawnPoints);
	DenominatorKillText->GetComponentByClass<UTextRenderComponent>()->SetText(FText::AsNumber(SpawnAmount));
	if(EasyButton && MediumButton && HardButton && StartButton)
	{
		EasyButton->ButtonPress.AddDynamic(this, &AGunRangeController::ChangeDifficulty);
		MediumButton->ButtonPress.AddDynamic(this, &AGunRangeController::ChangeDifficulty);
		HardButton->ButtonPress.AddDynamic(this, &AGunRangeController::ChangeDifficulty);
		StartButton->ButtonStart.AddDynamic(this, &AGunRangeController::StartGunRange);
	}
}


void AGunRangeController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGunRangeController::SpawnEnemyAtRandomPoint()
{
	if (SpawnPoints.Num() > 0)
	{
		int32 Index = FMath::RandRange(0, SpawnPoints.Num() - 1);
		FVector SpawnPosition = SpawnPoints[Index]->GetActorLocation();

		ATarget* Target = GetWorld()->SpawnActor<ATarget>(TargetClass, SpawnPosition, FRotator(0.0f, 180.0f, 0.0f));
		Target->OnDeath.AddDynamic(this, &AGunRangeController::KillCounter);
		Target->SetLifeSpan(SpawnInterval);

		SpawnCount++; 
		
		if (SpawnCount >= SpawnAmount)
		{
			GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
			
		}
		
	}
}

void AGunRangeController::KillCounter()
{
	KillCount++;
	KillText->GetComponentByClass<UTextRenderComponent>()->SetText(FText::AsNumber(KillCount));
}

void AGunRangeController::StartGunRange()
{
	ResetGunRange();
	KillText->GetComponentByClass<UTextRenderComponent>()->SetText(FText::AsNumber(KillCount));
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AGunRangeController::SpawnEnemyAtRandomPoint, SpawnInterval, true);
}

void AGunRangeController::ChangeDifficulty(float Interval)
{
	SpawnInterval = Interval;
}

void AGunRangeController::ResetGunRange()
{
	SpawnCount = 0;
	KillCount = 0;
}

