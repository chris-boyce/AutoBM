// Fill out your copyright notice in the Description page of Project Settings.


#include "GunRangeSpawnPoint.h"

// Sets default values
AGunRangeSpawnPoint::AGunRangeSpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGunRangeSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGunRangeSpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

