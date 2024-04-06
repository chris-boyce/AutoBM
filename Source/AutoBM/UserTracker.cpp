// Fill out your copyright notice in the Description page of Project Settings.


#include "UserTracker.h"




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
	GetWorld()->GetTimerManager().SetTimer(SphereTimerHandle, this, &UUserTracker::ViewPortChecker, 0.5f, true);
	GetWorld()->GetTimerManager().SetTimer(LineTraceTimerHandle, this, &UUserTracker::IsBeingBlocked, 0.05f, true);
}

void UUserTracker::StopTracking()
{
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

void UUserTracker::BotDamage(ATarget* Target)
{
	TimeToDamage.Add(GetWorld()->GetTimeSeconds() - StartTimeMap[Target]);
	DamageToKillTimeMap.Add(Target,GetWorld()->GetTimeSeconds());
}

