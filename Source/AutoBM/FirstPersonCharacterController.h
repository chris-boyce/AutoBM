// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FirstPersonCharacterController.generated.h"

struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
class AFirstPersonPlayer;

UCLASS()
class AUTOBM_API AFirstPersonCharacterController : public APlayerController
{
public:
	GENERATED_BODY()
	//Input Mapping
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="First Person Player|Character Movement")
	TObjectPtr<UInputMappingContext> InputMappingContext = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="First Person Player|Character Movement")
	TObjectPtr<UInputAction> ActionMove;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="First Person Player|Character Movement")
	TObjectPtr<UInputAction> ActionLook;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="First Person Player|Character Movement")
	TObjectPtr<UInputAction> ActionJump;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="First Person Player|Character Movement")
	TObjectPtr<UInputAction> ActionSprintDown;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="First Person Player|Character Movement")
	TObjectPtr<UInputAction> ActionSprintUp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="First Person Player|Character Movement")
	TObjectPtr<UInputAction> ActionCrouchDown;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="First Person Player|Character Movement")
	TObjectPtr<UInputAction> ActionCrouchUp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="First Person Player|Character Movement")
	TObjectPtr<UInputAction> ActionInteract;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="First Person Player|Character Movement")
	TObjectPtr<UInputAction> ActionFire;

	UFUNCTION()
	void AllowInput(bool bAllowMove);

	UFUNCTION()
	void MouseVisibility(bool bIsVisable);

protected:
	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;

	void HandleLook(const FInputActionValue& InputActionValue);
	void HandleMove(const FInputActionValue& InputActionValue);
	void HandleJump();
	void HandleSprintDown();
	void HandleSprintUp();
	void HandleCrouchDown();
	void HandleCrouchUp();
	void HandleInteract();
	void HandleFire();


private:
    UPROPERTY()
    UEnhancedInputComponent* EnhancedInputComponent = nullptr;
    
    UPROPERTY()
    AFirstPersonPlayer* PlayerCharacter = nullptr;
    
    UPROPERTY()
    bool bPlayerCanMove = true;
};
