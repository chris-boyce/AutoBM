// Fill out your copyright notice in the Description page of Project Settings.


#include "GunRangeButton.h"

AGunRangeButton::AGunRangeButton()
{


}

void AGunRangeButton::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGunRangeButton::Interact()
{
	if(!bIsStartButton)
	{
		ButtonPress.Broadcast(Speed);
	}
	else
	{
		ButtonStart.Broadcast();
	}
	
}

