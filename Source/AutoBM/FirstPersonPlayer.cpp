// Project 3 @Chris__boyce


#include "FirstPersonPlayer.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"

AFirstPersonPlayer::AFirstPersonPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AFirstPersonPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (RifleClass)
	{
		Rifle = GetWorld()->SpawnActor<ARifle>(RifleClass, FVector::ZeroVector, FRotator::ZeroRotator);

		if (Rifle)
		{
			FName SocketName = TEXT("r_socket"); 
			Rifle->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
		}
	}
	
	if(GetMovementComponent())
	{
		GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	}
	CameraComponent = FindComponentByClass<UCameraComponent>();
	
}

void AFirstPersonPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFirstPersonPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AFirstPersonPlayer::Jump()
{
	UnCrouch();
	Super::Jump();
}

void AFirstPersonPlayer::Crouch(bool bCilentSimulation)
{
	Super::Crouch(bCilentSimulation);
}

void AFirstPersonPlayer::AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce)
{
	Super::AddMovementInput(WorldDirection, ScaleValue, bForce);
}

void AFirstPersonPlayer::AttachGun()
{
	//Add Gun Attachment
}

void AFirstPersonPlayer::ToggleRunning()
{
	GetCharacterMovement()->MaxWalkSpeed = RunningMaxWalkSpeed;
}

void AFirstPersonPlayer::ToggleRunningOff()
{
	GetCharacterMovement()->MaxWalkSpeed = NormalMaxWalkSpeed;
}






