// Fill out your copyright notice in the Description page of Project Settings.


#include "UserTracker.h"

#include "EngineUtils.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "FirstPersonPlayer.h"
#include "GameFramework/PlayerStart.h"


UUserTracker::UUserTracker()
{

	PrimaryComponentTick.bCanEverTick = true;
	TargetClass = ATarget::StaticClass();
	
	
}

void UUserTracker::BeginPlay()
{
	Super::BeginPlay();
	
	
}


void UUserTracker::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	
}

void UUserTracker::StartTracking()
{
	UE_LOG(LogTemp, Warning, TEXT("Started tracking"));
	GetWorld()->GetTimerManager().SetTimer(SphereTimerHandle, this, &UUserTracker::ViewPortChecker, 0.5f, true);
	GetWorld()->GetTimerManager().SetTimer(LineTraceTimerHandle, this, &UUserTracker::IsBeingBlocked, 0.05f, true);
	auto temp = Cast<AFirstPersonPlayer>(GetOwner());
	if(temp)
	{
		UE_LOG(LogTemp, Warning, TEXT("Has temp"));
		Gun = temp->Rifle;
	}


	if(Gun)
	{
		UE_LOG(LogTemp, Warning, TEXT("Has Gun"));
		Gun->BulletMissed.AddDynamic(this, &UUserTracker::MissedBullet);
		Gun->Headshot.AddDynamic(this, &UUserTracker::HitHead);
		Gun->BodyShot.AddDynamic(this, &UUserTracker::HitBody);
		Gun->OtherShot.AddDynamic(this, &UUserTracker::HitOther);
		Gun->FiringMoving.AddDynamic(this, &UUserTracker::FiringWhenMoving);
		Gun->FiringStopped.AddDynamic(this, &UUserTracker::FiringWhenStopped);
	}

		
	
	
}

void UUserTracker::StopTracking()
{
	UE_LOG(LogTemp, Log, TEXT("Total Shots: %d"), TotalShot);
	UE_LOG(LogTemp, Log, TEXT("Other Shots: %d"), OtherShot);
	UE_LOG(LogTemp, Log, TEXT("Body Shots: %d"), BodyShot);
	UE_LOG(LogTemp, Log, TEXT("Head Shots: %d"), HeadShot);
	UE_LOG(LogTemp, Log, TEXT("Missed Shots: %d"), MissedShot);

	float TotalAccuracy = 0.0f;
	TotalAccuracy = (static_cast<float>(TotalShot - MissedShot) / TotalShot) * 100;
	UE_LOG(LogTemp, Log, TEXT("Total Accuracy: %f"), TotalAccuracy);

	int HitShots = HeadShot + BodyShot + OtherShot;
	float HeadshotPercentage = 0.0f;
	HeadshotPercentage = (static_cast<float>(HeadShot) / static_cast<float>(HitShots)) * 100.0f;
	UE_LOG(LogTemp, Log, TEXT("Headshot Percentage: %f"), HeadshotPercentage);

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

	float FirstBulletToKill = 0.0f;
	for (float Time : DamageToKillTime)
	{
		FirstBulletToKill += Time;
	}

	float AverageFirstBulletToKill = FirstBulletToKill / DamageToKillTime.Num();

	int total = FireMovingCount + FireStopCount;

	float FiringWhenMovingPercentage = static_cast<float>(FireMovingCount) / total;

	FiringWhenMovingPercentage *= 100.0f;

	float NormalizedTotalAccuracy = TotalAccuracy / 100.0f; //Higher is Better
	float NormalizedHeadshotPercentage = HeadshotPercentage / 100.0f; 
	float NormalizedTTK = 1.0f - (AverageKillTime / 5.0f);  //Lower is better
	float NormalizedTTD = 1.0f - (AverageTimeToDamage / 3.0f); 
	float NormalizedFBD = 1.0f - (AverageFirstBulletToKill / 4.0f); 
	float NormalizedFMP = 1.0f - (FiringWhenMovingPercentage / 100.0f);
	
	float TotalScore = 0;
	TotalScore = TotalScore + (NormalizedTotalAccuracy * 0.3f);
	TotalScore = TotalScore + (NormalizedHeadshotPercentage * 0.2f);
	TotalScore = TotalScore + (NormalizedTTK * 0.2f);
	TotalScore = TotalScore + (NormalizedTTD * 0.05f);
	TotalScore = TotalScore + (NormalizedFBD * 0.2f);
	TotalScore = TotalScore + (NormalizedFMP * 0.05f);
	
	UE_LOG(LogTemp, Log, TEXT("NormalFMP: %f"), NormalizedFMP);
	UE_LOG(LogTemp, Log, TEXT("NormalAcc: %f"), NormalizedTotalAccuracy);
	UE_LOG(LogTemp, Log, TEXT("NormalHSP: %f"), NormalizedHeadshotPercentage);
	UE_LOG(LogTemp, Log, TEXT("NormalTTK: %f"), NormalizedTTK);
	UE_LOG(LogTemp, Log, TEXT("NormalTTK: %f"), NormalizedTTD);
	UE_LOG(LogTemp, Log, TEXT("NormalFBD: %f"), NormalizedFBD);
	UE_LOG(LogTemp, Warning, TEXT("Score : %f"), TotalScore);

	FString HasFinishedString = "true";

	FString DataLine = FString::Printf(TEXT("%s, %f, %f, %f, %f, %f, %f, %s, %f\n"), *PlayerName, TotalAccuracy, HeadshotPercentage, AverageKillTime, AverageTimeToDamage, AverageFirstBulletToKill, FiringWhenMovingPercentage, *HasFinishedString, TotalScore);

	FString FilePath = FPaths::ProjectSavedDir() + TEXT("BotData.csv");

	bool bAppendToFile = true;
	
	FFileHelper::SaveStringToFile(DataLine, *FilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), bAppendToFile ? FILEWRITE_Append : FILEWRITE_None);
	
	UnbindDelegatesAndTimers();
	ResetPlayerPos();
}

void UUserTracker::UnbindDelegatesAndTimers()
{
	Gun->BulletMissed.RemoveDynamic(this, &UUserTracker::MissedBullet);
	Gun->Headshot.RemoveDynamic(this, &UUserTracker::HitHead);
	Gun->BodyShot.RemoveDynamic(this, &UUserTracker::HitBody);
	Gun->OtherShot.RemoveDynamic(this, &UUserTracker::HitOther);
	Gun->FiringMoving.RemoveDynamic(this, &UUserTracker::FiringWhenMoving);
	Gun->FiringStopped.RemoveDynamic(this, &UUserTracker::FiringWhenStopped);
	
	GetWorld()->GetTimerManager().ClearTimer(LineTraceTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(SphereTimerHandle);
	
	TotalShot = 0;
	HeadShot = 0;
	BodyShot = 0;
	OtherShot = 0;
	MissedShot = 0;
	FireMovingCount = 0;
	FireStopCount = 0;

	SeenTargets.Empty();
	InSenseRadius.Empty();

	StartTimeMap.Empty();
	DamageTimeMap.Empty();
	DamageToKillTimeMap.Empty();

	TimeToDamage.Empty();
	DamageToKillTime.Empty();
	KilledTime.Empty();
}

void UUserTracker::ResetPlayerPos()
{
	APlayerStart* StartPos = nullptr;
	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		StartPos = *It;
		break; 
	}
	if (StartPos)
	{
		FVector StartLocation = StartPos->GetActorLocation();
		FRotator StartRotation = StartPos->GetActorRotation();
		GetOwner()->SetActorLocationAndRotation(StartLocation, StartRotation);
	}
}


void UUserTracker::ViewPortChecker()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if(PlayerController)
	{
		InSenseRadius.Empty();
		FVector PlayerViewLocation;
		FRotator PlayerViewRotation;
		PlayerController->GetPlayerViewPoint(PlayerViewLocation, PlayerViewRotation);

		FVector ForwardVector = PlayerViewRotation.Vector();
		FVector StartPoint = PlayerViewLocation + (ForwardVector * 2000.0f); // Start 1000 units in front
		float Radius = 2000.0f; // Sphere radius

		TArray<FOverlapResult> OverlapResults;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(GetOwner());
		QueryParams.bTraceComplex = true;

		if(GetWorld()->OverlapMultiByObjectType(OverlapResults, StartPoint, FQuat::Identity, FCollisionObjectQueryParams(ECollisionChannel::ECC_Pawn), FCollisionShape::MakeSphere(Radius), QueryParams))
		{
			for(const FOverlapResult& Result : OverlapResults)
			{
				AActor* OverlappedActor = Result.GetActor();
				if(OverlappedActor && OverlappedActor->IsA(TargetClass))
				{
					if(!InSenseRadius.Contains(OverlappedActor))
					{
						
						InSenseRadius.Add(OverlappedActor);
						
					}
				}
			}
		}
	}
    
}

void UUserTracker::SubToTarget(ATarget* Target)
{
	Target->OnUserDeath.AddDynamic(this, &UUserTracker::BotDeath);
	Target->OnUserDamage.AddDynamic(this, &UUserTracker::BotDamage);
}

void UUserTracker::BotDeath(ATarget* Target)
{
	UE_LOG(LogTemp, Warning, TEXT("Yeeeet"));
	KilledTime.Add(GetWorld()->GetTimeSeconds() - StartTimeMap[Target]);
	DamageToKillTime.Add(GetWorld()->GetTimeSeconds() - DamageToKillTimeMap[Target]);
	StartTimeMap.Remove(Target);
	SeenTargets.Remove(Target);
}

void UUserTracker::IsBeingBlocked()
{
	
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if(PlayerController)
	{
		FVector PlayerViewLocation;
		FRotator PlayerViewRotation;
		PlayerController->GetPlayerViewPoint(PlayerViewLocation, PlayerViewRotation);
		for(AActor* TargetActor : InSenseRadius)
		{
			
			if(TargetActor)
			{
				FVector TargetLocation = TargetActor->GetActorLocation();
				FHitResult HitResult;
				FCollisionQueryParams QueryParams;
				QueryParams.AddIgnoredActor(PlayerController->GetPawn()); 
				QueryParams.bTraceComplex = true;
				
				bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, PlayerViewLocation, TargetLocation, ECC_Visibility, QueryParams);
				
				if(bHit)
				{
					if(HitResult.GetActor() == TargetActor)
					{
						auto temp = Cast<ATarget>(TargetActor);
						if(!SeenTargets.Contains(temp))
						{
							SeenTargets.Add(temp);
							SubToTarget(temp);
							StartTimer(temp);
							UE_LOG(LogTemp, Warning, TEXT("Seen Actor: %s"), *TargetActor->GetName());
						}
					}
				}
			}
		}
	}
}

void UUserTracker::StartTimer(ATarget* Target)
{
	StartTimeMap.Add(Target, GetWorld()->GetTimeSeconds());
}

void UUserTracker::MissedBullet()
{
	TotalShot++;
	MissedShot++;
}

void UUserTracker::HitHead()
{
	TotalShot++;
	HeadShot++;
}

void UUserTracker::HitBody()
{
	TotalShot++;
	BodyShot++;
}

void UUserTracker::HitOther()
{
	TotalShot++;
	OtherShot++;
}

void UUserTracker::FiringWhenMoving()
{
	FireMovingCount++;
}

void UUserTracker::FiringWhenStopped()
{
	FireStopCount++;
}

void UUserTracker::BotDamage(ATarget* Target)
{
	TimeToDamage.Add(GetWorld()->GetTimeSeconds() - StartTimeMap[Target]);
	DamageToKillTimeMap.Add(Target,GetWorld()->GetTimeSeconds());
}

