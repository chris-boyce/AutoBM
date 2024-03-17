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
	if(PlayerCharacter)
	{
		PlayerCharacter->OnWeaponPickup.AddDynamic(this, &AFirstPersonCharacterController::AddBinds);
		UE_LOG(LogTemp, Warning, TEXT("YEP WE DID IT"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PLAYERCHAR ISNT AROUND BUDDY"));
	}
	
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
	PlayerCharacter->PickUp();
	
}

void AFirstPersonCharacterController::HandleFireDown()
{
	if(!bPlayerCanMove)
	{
		return;
	}
	if(PlayerCharacter->Rifle)
	{
		PlayerCharacter->Rifle->StartFiring();
	}
	
}

void AFirstPersonCharacterController::HandleFireUp()
{
	if(PlayerCharacter->Rifle)
	{
		PlayerCharacter->Rifle->StopFiring();
	}
	
}

void AFirstPersonCharacterController::HandleReload()
{
	if(!bPlayerCanMove)
	{
		return;
	}
	if(PlayerCharacter->Rifle)
	{
		PlayerCharacter->Rifle->Reload();
	}
	
}

void AFirstPersonCharacterController::AddBinds()
{
	
	UE_LOG(LogTemp, Warning, TEXT("Add Binds Run"));
	if(PlayerCharacter->Rifle)
	{
		UE_LOG(LogTemp, Warning, TEXT("Rifle Found"));
		PlayerCharacter->Rifle->WeaponUpdateAmmoHUD.AddDynamic(FirstPersonHUD, &UFirstPersonWidget::UpdateAmmo);
		PlayerCharacter->Rifle->WeaponUpdateReloadTime.AddDynamic(FirstPersonHUD, &UFirstPersonWidget::UpdateReloadBar);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Rifle Not Found"));
	}
}





