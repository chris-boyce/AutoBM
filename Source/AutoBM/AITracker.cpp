// Fill out your copyright notice in the Description page of Project Settings.


#include "AITracker.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"


UAITracker::UAITracker()
{

	PrimaryComponentTick.bCanEverTick = true;

}


void UAITracker::BeginPlay()
{
	Super::BeginPlay();


}


void UAITracker::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UAITracker::StartTracking(AAIBot* AIComp, ATarget* Bot, AAIRifle* AIRifle)
{
	AIRifle->BulletMissed.AddDynamic(this, &UAITracker::MissedBullet);
	AIRifle->Headshot.AddDynamic(this, &UAITracker::HitHead);
	AIRifle->BodyShot.AddDynamic(this, &UAITracker::HitBody);
	AIRifle->OtherShot.AddDynamic(this, &UAITracker::HitOther);

	AIComp->SeenEnemy.AddDynamic(this, &UAITracker::SeenTimer);
	AIComp->KilledEnemy.AddDynamic(this, &UAITracker::KilledTimerStop);

	Bot->OnDeath.AddDynamic(this, &UAITracker::OnCompletedCourse);

	BotName = AIComp->BotName; 
}

void UAITracker::MissedBullet()
{
	TotalShot++;
	MissedShot++;
	UE_LOG(LogTemp, Log, TEXT("Missed Shots: %d"), MissedShot);
}

void UAITracker::HitHead()
{
	TotalShot++;
	HeadShot++;
	UE_LOG(LogTemp, Log, TEXT("Head Shots: %d"), HeadShot);
	TimeToDamageStop();
}

void UAITracker::HitBody()
{
	TotalShot++;
	BodyShot++;
	UE_LOG(LogTemp, Log, TEXT("Body Shots: %d"), BodyShot);
	TimeToDamageStop();
}

void UAITracker::HitOther()
{
	TotalShot++;
	OtherShot++;
	UE_LOG(LogTemp, Log, TEXT("Other Shots: %d"), OtherShot);
	TimeToDamageStop();
}

void UAITracker::SeenTimer()
{
	StartTime = GetWorld()->GetTimeSeconds();
}

void UAITracker::KilledTimerStop()
{
	KilledTime.Add(GetWorld()->GetTimeSeconds() - StartTime);

	UE_LOG(LogTemp, Error, TEXT("Elapsed Time: %f seconds"), GetWorld()->GetTimeSeconds() - StartTime);
	FirstHit = true;
}

void UAITracker::TimeToDamageStop()
{
	if(FirstHit)
	{
		TimeToDamage.Add(GetWorld()->GetTimeSeconds() - StartTime);

		UE_LOG(LogTemp, Log, TEXT("Elapsed Time To Damage: %f seconds"), GetWorld()->GetTimeSeconds() - StartTime);
		FirstHit = false;
	}
	
}

void UAITracker::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	UE_LOG(LogTemp, Log, TEXT("Total Shots: %d"), TotalShot);
	UE_LOG(LogTemp, Log, TEXT("Other Shots: %d"), OtherShot);
	UE_LOG(LogTemp, Log, TEXT("Body Shots: %d"), BodyShot);
	UE_LOG(LogTemp, Log, TEXT("Head Shots: %d"), HeadShot);
	UE_LOG(LogTemp, Log, TEXT("Missed Shots: %d"), MissedShot);
	
	//Accuracy
	float TotalAccuracy = 0.0f;
	TotalAccuracy = (static_cast<float>(TotalShot - MissedShot) / TotalShot) * 100;
	UE_LOG(LogTemp, Log, TEXT("Total Accuracy: %f"), TotalAccuracy);

	//Headshot Percentage
	int HitShots = HeadShot + BodyShot + OtherShot;
	float HeadshotPercentage = 0.0f;
	HeadshotPercentage = (static_cast<float>(HeadShot) / static_cast<float>(HitShots)) * 100.0f;
	UE_LOG(LogTemp, Log, TEXT("Headshot Percentage: %f"), HeadshotPercentage);

	//Kill Time
	float TotalKillTime = 0.0f;
	for (float Time : KilledTime)
	{
		TotalKillTime += Time;
	}

	float AverageKillTime = TotalKillTime / KilledTime.Num();

	float TotalTimeToDamage = 0.0f;
	for (float Time : TimeToDamage)
	{
		TotalTimeToDamage += Time;
	}

	float AverageTimeToDamage = TotalTimeToDamage / TimeToDamage.Num();

	FString HasFinishedString = HasFinished ? TEXT("True") : TEXT("False");

	FString DataLine = FString::Printf(TEXT("%s, %f, %f, %f, %f, %s\n"), *BotName, TotalAccuracy, HeadshotPercentage, AverageKillTime, AverageTimeToDamage, *HasFinishedString);

	FString FilePath = FPaths::ProjectSavedDir() + TEXT("BotData.csv");

	bool bAppendToFile = true;
	
	FFileHelper::SaveStringToFile(DataLine, *FilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), bAppendToFile ? FILEWRITE_Append : FILEWRITE_None);
	Super::OnComponentDestroyed(bDestroyingHierarchy);
}

void UAITracker::OnCompletedCourse()
{
	HasFinished = true;
}

