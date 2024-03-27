// Fill out your copyright notice in the Description page of Project Settings.


#include "AITracker.h"


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

void UAITracker::StartTracking(AAIBot* AIComp, AAIRifle* AIRifle)
{
	AIRifle->BulletMissed.AddDynamic(this, &UAITracker::MissedBullet);
	AIRifle->Headshot.AddDynamic(this, &UAITracker::HitHead);
	AIRifle->BodyShot.AddDynamic(this, &UAITracker::HitBody);
	AIRifle->OtherShot.AddDynamic(this, &UAITracker::HitOther);
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
}

void UAITracker::HitBody()
{
	TotalShot++;
	BodyShot++;
	UE_LOG(LogTemp, Log, TEXT("Body Shots: %d"), BodyShot);
}

void UAITracker::HitOther()
{
	TotalShot++;
	OtherShot++;
	UE_LOG(LogTemp, Log, TEXT("Other Shots: %d"), OtherShot);
}

void UAITracker::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	UE_LOG(LogTemp, Log, TEXT("Total Shots: %d"), TotalShot);
	UE_LOG(LogTemp, Log, TEXT("Other Shots: %d"), OtherShot);
	UE_LOG(LogTemp, Log, TEXT("Body Shots: %d"), BodyShot);
	UE_LOG(LogTemp, Log, TEXT("Head Shots: %d"), HeadShot);
	UE_LOG(LogTemp, Log, TEXT("Missed Shots: %d"), MissedShot);
	Super::OnComponentDestroyed(bDestroyingHierarchy);
}

