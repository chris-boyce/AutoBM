// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIPath.generated.h"

UCLASS()
class AUTOBM_API AAIPath : public AActor
{
	GENERATED_BODY()
	
public:	

	AAIPath();

protected:

	virtual void BeginPlay() override;
public:
	
	UFUNCTION()
	FVector GetPatrolPoint(int const index) const;

	UFUNCTION()
	int Num() const;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(MakeEditWidget="true", AllowPrivateAccess="true"))
	TArray<FVector> PatrolPoints;

};
