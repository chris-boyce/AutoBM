// Project 3 @Chris__boyce


#include "FirstPersonPlayer.h"

#include "Pickupable.h"
#include "Camera/CameraComponent.h"
#include "Rifle.h"
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
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

}

void AFirstPersonPlayer::BeginPlay()
{
	Super::BeginPlay();

	CharacterMovementComp = GetCharacterMovement();
	
	
	if(GetMovementComponent())
	{
		GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	}
	CameraComponent = FindComponentByClass<UCameraComponent>();
	
}

void AFirstPersonPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CameraBob();
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
	if (RifleClass)
	{
		Rifle = GetWorld()->SpawnActor<ARifle>(RifleClass, FVector::ZeroVector, FRotator::ZeroRotator);

		if (Rifle)
		{
			FName SocketName = TEXT("r_socket");
			Rifle->Player = this;
			Rifle->AttachToComponent(Mesh1P, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
			Rifle->WeaponFired.AddDynamic(this, &AFirstPersonPlayer::GunFired);
			Rifle->WeaponReload.AddDynamic(this, &AFirstPersonPlayer::WeaponReload);
			OnWeaponPickup.Broadcast();
			Rifle->WeaponUpdateAmmoHUD.Broadcast(Rifle->CurrentAmmo, Rifle->FullAmmo);
		}
	}
}

void AFirstPersonPlayer::ToggleRunning()
{
	GetCharacterMovement()->MaxWalkSpeed = RunningMaxWalkSpeed;
}

void AFirstPersonPlayer::ToggleRunningOff()
{
	GetCharacterMovement()->MaxWalkSpeed = NormalMaxWalkSpeed;
}

float AFirstPersonPlayer::GetMovementSpeed()
{
	if (CharacterMovementComp != nullptr)
	{
		return CharacterMovementComp->Velocity.Size();
	}
	
	return 0.0f;
}

void AFirstPersonPlayer::CameraBob()
{
	bIsWalking = (GetMovementSpeed() > 10);
	OriginalCameraOffset = bIsWalking ? FVector::ZeroVector : OriginalCameraOffset;
	if (bIsWalking)
	{
		FVector CameraOffset = FVector(0.0f, 0.0f, 0.0f); 
		float BobbingAmount = FMath::Sin(GetWorld()->GetTimeSeconds() * BobbingSpeed) * BobbingIntensity;
		CameraOffset.Z += BobbingAmount;
		if (CameraComponent)
		{
			CameraComponent->SetRelativeLocation(CameraOffset);
		}
	}
	else
	{
		if (CameraComponent)
		{
			float ResetInterpSpeed = 1.0f;
			FVector CurrentCameraOffset = CameraComponent->GetRelativeLocation();
			FVector NewCameraOffset = FMath::VInterpTo(CurrentCameraOffset, OriginalCameraOffset, GetWorld()->GetDeltaSeconds(), ResetInterpSpeed);
			CameraComponent->SetRelativeLocation(NewCameraOffset);
		}
	}
	
}

void AFirstPersonPlayer::WeaponReload()
{
	UE_LOG(LogTemp, Warning, TEXT("Player Reload"));
	Mesh1P->PlayAnimation(ReloadGunAnim, false);
}

void AFirstPersonPlayer::PickUp()
{
	UE_LOG(LogTemp, Warning, TEXT("Pickup Called"));
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	FVector StartLocation; 
	FRotator StartRotation;
	PlayerController->GetPlayerViewPoint(StartLocation, StartRotation);
	FVector FiringDirection = StartRotation.Vector();
	FVector EndLocation = StartLocation + (FiringDirection * 500); 
	
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); 
	QueryParams.AddIgnoredActor(GetOwner()); 
	QueryParams.bTraceComplex = true;

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams);

	if (bHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("HAS HIT"));
		auto HitComponent = HitResult.GetComponent();
		if (HitComponent)
		{
			AActor* HitActor = HitResult.GetActor();
			IPickupable* HitHandler = Cast<IPickupable>(HitActor);
			if (HitHandler)
			{
				UE_LOG(LogTemp, Warning, TEXT("Has Hit Rifle"));
				RifleClass = HitHandler->Pickup();
				AttachGun();
				
			}
		}
	}
}


void AFirstPersonPlayer::GunFired()
{
	Mesh1P->PlayAnimation(FireGunAnim, false);
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerController->ClientStartCameraShake(FiringGun, 10);
}






