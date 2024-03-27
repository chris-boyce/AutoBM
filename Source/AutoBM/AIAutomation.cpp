// Fill out your copyright notice in the Description page of Project Settings.


#include "AIAutomation.h"


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
	
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AAIAutomation::OnOverlapBegin);
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
		CurrentBotController->InitializeController(20,5,0.5,AimsCurves[0], VarietyCurve[0],0.3,0.7,0.1,0.3);
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		CurrentBot = GetWorld()->SpawnActor<ATarget>(BotClass, StartPoint, FRotator::ZeroRotator, SpawnParams);
		CurrentBotController->Possess(CurrentBot);
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
	UE_LOG(LogTemp, Warning, TEXT("NOPE"));
	if (OtherActor && OtherActor->IsA(ATarget::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("YEPPP"));
		CurrentBot->Death();
		TargetConstruction();
		BotConstruction();
	}
}

