// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIBot.h"

#include "Target.h"
#include "GameFramework/Actor.h"
#include "AIAutomation.generated.h"

class AAutoCam;
USTRUCT(BlueprintType)
struct FBotData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float HeadShotPercentage;

	UPROPERTY(EditAnywhere)
	int BulletMissResetAmount;

	UPROPERTY(EditAnywhere)
	float AimResetSpeed;

	UPROPERTY(EditAnywhere)
	int RecoilCurve;

	UPROPERTY(EditAnywhere)
	int VarietyCurve;

	UPROPERTY(EditAnywhere)
	float FiringReactionLower;

	UPROPERTY(EditAnywhere)
	float FiringReactionUpper;

	UPROPERTY(EditAnywhere)
	float WalkingReactionLower;

	UPROPERTY(EditAnywhere)
	float WalkingReactionUpper;
	
};


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
	TSubclassOf<ATarget> TargetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AAIBot> BotControllerClass;

	UPROPERTY(EditAnywhere)
	UDataTable* BotDataTable;

	UPROPERTY(VisibleAnywhere)
	TArray<FBotData> BotData;

	UPROPERTY()
	AAIBot* CurrentBotController;

	UPROPERTY()
	ATarget* CurrentBot;
	
	UPROPERTY(EditAnywhere)
	TArray<UCurveFloat*> AimsCurves;

	UPROPERTY(EditAnywhere)
	TArray<UCurveFloat*> VarietyCurve;
	
	UFUNCTION()
	void BotConstruction();

	UFUNCTION()
	void TargetConstruction();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere)
	int CurrentBotIndex = 0;

	UPROPERTY(EditAnywhere)
	bool RunSim = false;

	UPROPERTY(EditAnywhere)
	float TimeAcceleration = 1.0f;

	UPROPERTY()
	TArray<AAutoCam*> AutoCams;
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(MakeEditWidget="true", AllowPrivateAccess="true"))
	TArray<FVector> TargetPoints;
	
};
