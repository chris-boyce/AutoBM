// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Rifle.h"
#include "UObject/Interface.h"
#include "Shootable.generated.h"


UINTERFACE(MinimalAPI)
class UShootable : public UInterface
{
	GENERATED_BODY()
};


class AUTOBM_API IShootable
{
	GENERATED_BODY()

	
public:
	virtual void HandleHit(UPrimitiveComponent* HitComponent, FDamageInfo DamageInfo) = 0;
};
