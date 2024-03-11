// Project 3 @Chris__boyce

#pragma once

#include "CoreMinimal.h"
#include "Rifle.h"
#include "GameFramework/Character.h"
#include "FirstPersonPlayer.generated.h"


class UCameraComponent;
UCLASS()
class AUTOBM_API AFirstPersonPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	AFirstPersonPlayer();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Jump() override;

	virtual void Crouch(bool bCilentSimulation = false) override;

	virtual void AddMovementInput(FVector WorldDirection, float ScaleValue = 1.0f, bool bForce = false) override;

	UFUNCTION()
	void AttachGun();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Movement", meta = (AllowPrivateAccess = "true"))
	float NormalMaxWalkSpeed = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Movement", meta = (AllowPrivateAccess = "true"))
	float RunningMaxWalkSpeed = 1000.0f;

	UFUNCTION(BlueprintCallable, Category="Player|Movement")
	void ToggleRunning();

	UFUNCTION(BlueprintCallable, Category="Player|Movement")
	void ToggleRunningOff();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ARifle> RifleClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ARifle* Rifle = nullptr;

	bool bIsRunning = false;


	
	
};
