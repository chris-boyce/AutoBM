// Fill out your copyright notice in the Description page of Project Settings.


#include "AIPath.h"


AAIPath::AAIPath()
{

	PrimaryActorTick.bCanEverTick = false;

}


void AAIPath::BeginPlay()
{
	Super::BeginPlay();
	
}

FVector AAIPath::GetPatrolPoint(int const index) const
{
	return PatrolPoints[index];
}

int AAIPath::Num() const
{
	return PatrolPoints.Num();
}



