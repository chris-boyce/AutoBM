// Project 3 @Chris__boyce


#include "FirstPersonPlayer.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"

AFirstPersonPlayer::AFirstPersonPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

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
			Rifle->AttachToComponent(Mesh1P, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
			Rifle->WeaponFired.AddDynamic(this, &AFirstPersonPlayer::GunFired);
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

void AFirstPersonPlayer::GunFired()
{
	UE_LOG(LogTemp, Warning, TEXT("PLAYER HAS RECIEVED THIS"));
	Mesh1P->PlayAnimation(FireGun, false);
}






