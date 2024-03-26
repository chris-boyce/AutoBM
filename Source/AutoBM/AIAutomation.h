// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIBot.h"
#include "Target.h"
#include "GameFramework/Actor.h"
#include "AIAutomation.generated.h"

UCLASS()
class AUTOBM_API AAIAutomation : public AActor
{
	GENERATED_BODY()
	
public:	
	AAIAutomation();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* BoxCollider;
	
	UPROPERTY(EditAnywhere)
	FVector StartPoint;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ATarget> BotClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AAIBot> BotControllerClass;

	UPROPERTY()
	AAIBot* CurrentBotController;

	UPROPERTY()
	ATarget* CurrentBot;
	
	UPROPERTY(EditAnywhere)
	TArray<UCurveFloat*> AimsCurves;
	
	UFUNCTION()
	void BotConstruction();

	UFUNCTION()
	void TargetConstruction();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(MakeEditWidget="true", AllowPrivateAccess="true"))
	TArray<FVector> TargetPoints;
	
};
