// Fill out your copyright notice in the Description page of Project Settings.


#include "StartCourse.h"

#include "FirstPersonPlayer.h"
#include "UserTracker.h"


AStartCourse::AStartCourse()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	RootComponent = BoxCollider;
	
	BoxCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxCollider->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	BoxCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

}


void AStartCourse::BeginPlay()
{
	Super::BeginPlay();
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AStartCourse::OnOverlapBegin);
	
}


void AStartCourse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStartCourse::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!IsEndPoint)
	{
		if (OtherActor && OtherActor->IsA(AFirstPersonPlayer::StaticClass()))
		{
			UUserTracker* Tracker = OtherActor->GetComponentByClass<UUserTracker>();
			Tracker->StartTracking();
		}
	}
	else
	{
		if (OtherActor && OtherActor->IsA(AFirstPersonPlayer::StaticClass()))
		{
			UUserTracker* Tracker = OtherActor->GetComponentByClass<UUserTracker>();
			Tracker->StopTracking();
		}
	}
	
}



