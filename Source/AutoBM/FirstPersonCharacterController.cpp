// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstPersonCharacterController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Rifle.h"
#include "FirstPersonPlayer.h"
#include "FirstPersonWidget.h"


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

void AFirstPersonCharacterController::AddHUD()
{
	if (CrosshairWidgetClass)
	{
		CrosshairWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), CrosshairWidgetClass);
		if (CrosshairWidgetInstance)
		{
			CrosshairWidgetInstance->AddToViewport();
		}
	}
	if(FirstPersonHUDClass)
	{
		FirstPersonHUD = CreateWidget<UFirstPersonWidget>(GetWorld(), FirstPersonHUDClass);
		if (FirstPersonHUD)
		{
			FirstPersonHUD->AddToViewport();
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Bind HUD"));
	
	
}

void AFirstPersonCharacterController::AllowInput(bool bAllowMove)
{
	bPlayerCanMove = bAllowMove;
}


void AFirstPersonCharacterController::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AFirstPersonCharacterController::AddBinds, 0.1f, false);
}

void AFirstPersonCharacterController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	PlayerCharacter = Cast<AFirstPersonPlayer>(aPawn);
	
	AddHUD();
	
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
		EnhancedInputComponent->BindAction(ActionFireUp, ETriggerEvent::Triggered, this, &AFirstPersonCharacterController::HandleFireUp);
	}
	if(ActionReload)
	{
		EnhancedInputComponent->BindAction(ActionReload, ETriggerEvent::Triggered, this, &AFirstPersonCharacterController::HandleReload);
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
	PlayerCharacter->Rifle->StopFiring();
}

void AFirstPersonCharacterController::HandleReload()
{
	if(!bPlayerCanMove)
	{
		return;
	}
	PlayerCharacter->Rifle->Reload();
	UE_LOG(LogTemp, Warning, TEXT("REload Button Hit"));

	
	
}

void AFirstPersonCharacterController::AddBinds()
{
	if(PlayerCharacter->Rifle)
	{
		PlayerCharacter->Rifle->WeaponUpdateAmmoHUD.AddDynamic(FirstPersonHUD, &UFirstPersonWidget::UpdateAmmo);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Rifle Not Found"));
	}
}





