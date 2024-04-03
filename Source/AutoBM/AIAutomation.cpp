// Fill out your copyright notice in the Description page of Project Settings.


#include "AIAutomation.h"

#include "Kismet/GameplayStatics.h"


AAIAutomation::AAIAutomation()
{
	PrimaryActorTick.bCanEverTick = true;
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	RootComponent = BoxCollider;
	
	BoxCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxCollider->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	BoxCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

}

void AAIAutomation::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 3.0f);
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AAIAutomation::OnOverlapBegin);
	

	if (BotDataTable)
	{
		TArray<FName> RowNames = BotDataTable->GetRowNames();
		
		for (const FName& RowName : RowNames)
		{
			FBotData* RowData = BotDataTable->FindRow<FBotData>(RowName, TEXT(""));

			if (RowData)
			{
				BotData.Add(*RowData);
			}
		}
	}
	
	TargetConstruction();
	BotConstruction();
	
}

void AAIAutomation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAIAutomation::BotConstruction()
{
	CurrentBotController = GetWorld()->SpawnActor<AAIBot>(BotControllerClass, FVector::ZeroVector, FRotator::ZeroRotator);
	if(CurrentBotController)
	{
		int temp = BotData[CurrentBotIndex].RecoilCurve - 1;
		CurrentBotController->InitializeController(FString::FromInt(CurrentBotIndex), BotData[CurrentBotIndex].HeadShotPercentage,
									BotData[CurrentBotIndex].BulletMissResetAmount,BotData[CurrentBotIndex].AimResetSpeed,
									AimsCurves[temp], VarietyCurve[0],BotData[CurrentBotIndex].FiringReactionLower,
									BotData[CurrentBotIndex].FiringReactionUpper,BotData[CurrentBotIndex].WalkingReactionLower,
									BotData[CurrentBotIndex].WalkingReactionUpper);
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		CurrentBot = GetWorld()->SpawnActor<ATarget>(BotClass, StartPoint, FRotator::ZeroRotator, SpawnParams);
		CurrentBotController->Possess(CurrentBot);
	}
	CurrentBotIndex++;
	if(CurrentBotIndex == 100)
	{
		CurrentBotIndex = 0;
	}
}

void AAIAutomation::TargetConstruction()
{
	for (FVector TargetPoint : TargetPoints)
	{
		FVector WorldTargetPoint = ActorToWorld().TransformPosition(TargetPoint);
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		GetWorld()->SpawnActor<AActor>(TargetClass, WorldTargetPoint, FRotator::ZeroRotator, SpawnParams);
	}
}

void AAIAutomation::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->IsA(ATarget::StaticClass()))
	{
		CurrentBot->Death();
		TargetConstruction();
		BotConstruction();
	}
}

