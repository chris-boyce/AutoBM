// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstPersonCharacterController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FirstPersonPlayer.h"


void AFirstPersonCharacterController::MouseVisibility(bool bIsVisable)
{
	if (bIsVisable)
	{
		bShowMouseCursor = true;
		SetInputMode(FInputModeGameAndUI());
	}
	else
	{
		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
	}
}

void AFirstPersonCharacterController::AddCrosshair()
{
	if (CrosshairWidgetClass)
	{
		CrosshairWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), CrosshairWidgetClass);
		if (CrosshairWidgetInstance)
		{
			CrosshairWidgetInstance->AddToViewport();
		}
	}
}

void AFirstPersonCharacterController::AllowInput(bool bAllowMove)
{
	bPlayerCanMove = bAllowMove;
}

void AFirstPersonCharacterController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	PlayerCharacter = Cast<AFirstPersonPlayer>(aPawn);
	
	AddCrosshair();
	
	EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	InputSubsystem->ClearAllMappings();
	InputSubsystem->AddMappingContext(InputMappingContext, 0);
	
	if(ActionMove)
	{
		EnhancedInputComponent->BindAction(ActionMove, ETriggerEvent::Triggered, this, &AFirstPersonCharacterController::HandleMove);
	}
	if(ActionLook)
	{
		EnhancedInputComponent->BindAction(ActionLook, ETriggerEvent::Triggered, this, &AFirstPersonCharacterController::HandleLook);
	}
	if(ActionJump)
	{
		EnhancedInputComponent->BindAction(ActionJump, ETriggerEvent::Triggered, this, &AFirstPersonCharacterController::HandleJump);
	}
	if(ActionCrouchDown)
	{
		EnhancedInputComponent->BindAction(ActionCrouchDown, ETriggerEvent::Triggered, this, &AFirstPersonCharacterController::HandleCrouchDown);
	}
	if(ActionCrouchUp)
	{
		EnhancedInputComponent->BindAction(ActionCrouchUp, ETriggerEvent::Triggered, this, &AFirstPersonCharacterController::HandleCrouchUp);
	}
	if(ActionSprintDown)
	{
		EnhancedInputComponent->BindAction(ActionSprintDown, ETriggerEvent::Triggered, this, &AFirstPersonCharacterController::HandleSprintDown);
	}
	if(ActionSprintUp)
	{
		EnhancedInputComponent->BindAction(ActionSprintUp, ETriggerEvent::Triggered, this, &AFirstPersonCharacterController::HandleSprintUp);
	}
	if(ActionInteract)
	{
		EnhancedInputComponent->BindAction(ActionInteract, ETriggerEvent::Triggered, this, &AFirstPersonCharacterController::HandleInteract);
	}
	if(ActionFireDown)
	{
		EnhancedInputComponent->BindAction(ActionFireDown, ETriggerEvent::Triggered, this, &AFirstPersonCharacterController::HandleFireDown);
	}
	if(ActionFireUp)
	{
		UE_LOG(LogTemp, Error, TEXT("KEYBound"));
		EnhancedInputComponent->BindAction(ActionFireUp, ETriggerEvent::Triggered, this, &AFirstPersonCharacterController::HandleFireUp);
	}
	
}

void AFirstPersonCharacterController::OnUnPossess()
{
	EnhancedInputComponent->ClearActionBindings();
	Super::OnUnPossess();
}

void AFirstPersonCharacterController::HandleLook(const FInputActionValue& InputActionValue)
{
	if(!bPlayerCanMove)
	{
		return;
	}
	const FVector2D LookVector = InputActionValue.Get<FVector2D>();
	AddYawInput(LookVector.X);
	AddPitchInput(LookVector.Y);
}

void AFirstPersonCharacterController::HandleMove(const FInputActionValue& InputActionValue)
{
	if(!bPlayerCanMove)
	{
		return;
	}
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
	if (PlayerCharacter)
	{
		PlayerCharacter->AddMovementInput(PlayerCharacter->GetActorForwardVector(), MovementVector.Y);
		PlayerCharacter->AddMovementInput(PlayerCharacter->GetActorRightVector(), MovementVector.X);
	}
}

void AFirstPersonCharacterController::HandleJump()
{
	if(!bPlayerCanMove)
	{
		return;
	}
	if(!bPlayerCanMove)
	{
		return;
	}
	if(PlayerCharacter)
	{
		PlayerCharacter->UnCrouch();
		PlayerCharacter->Jump();
	}
}

void AFirstPersonCharacterController::HandleSprintDown()
{
	if(!bPlayerCanMove)
	{
		return;
	}
	if (PlayerCharacter)
	{
		PlayerCharacter->ToggleRunning();
	}
}

void AFirstPersonCharacterController::HandleSprintUp()
{
	if(!bPlayerCanMove)
	{
		return;
	}
	if (PlayerCharacter)
	{
		PlayerCharacter->ToggleRunningOff();
	}
}

void AFirstPersonCharacterController::HandleCrouchDown()
{
	if(!bPlayerCanMove)
	{
		return;
	}
	PlayerCharacter->Crouch();
	
}

void AFirstPersonCharacterController::HandleCrouchUp()
{
	if(!bPlayerCanMove)
	{
		return;
	}
	PlayerCharacter->UnCrouch();
}

void AFirstPersonCharacterController::HandleInteract()
{
	if(!bPlayerCanMove)
	{
		return;
	}
	//if(PlayerCharacter)
	//{
		//PlayerCharacter->InteractionComponent->FireInteraction();
	//}
}

void AFirstPersonCharacterController::HandleFireDown()
{
	if(!bPlayerCanMove)
	{
		return;
	}
	PlayerCharacter->Rifle->StartFiring();
}

void AFirstPersonCharacterController::HandleFireUp()
{
	UE_LOG(LogTemp, Error, TEXT("KEY REGISTERED"));
	PlayerCharacter->Rifle->StopFiring();
}





