// Copyright Epic Games, Inc. All Rights Reserved.

#include "PacManVRCharacter.h"

#include "AOfficer.h"
#include "PacManVRProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"

#include "AOfficer.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// APacManVRCharacter

APacManVRCharacter::APacManVRCharacter()
{
	// Set size for collision capsule
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

	PrimaryActorTick.bCanEverTick = true;

	bIsEventBound = false;


	
}

void APacManVRCharacter::EnterInteractionState()
{
	CurrentState = EPlayerState::Interacting;
	GetCharacterMovement()->DisableMovement();
	//Disable player controller input

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		PC->SetIgnoreLookInput(true);
	}
}

void APacManVRCharacter::ExitInteractionState()
{
	CurrentState = EPlayerState::Free;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		PC->SetIgnoreLookInput(false);
	}
}


void APacManVRCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

void APacManVRCharacter::PerformRaycast()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController == nullptr)
	{
		UE_LOG(LogTemplateCharacter, Warning, TEXT("No Player Controller Found!"));

		return;
	}

	FVector ViewLocation;
	FRotator ViewRotation;
	PlayerController->GetPlayerViewPoint(ViewLocation,ViewRotation);

	FVector RayStart = ViewLocation;
	FVector RayEnd = ViewLocation + ViewRotation.Vector() * 1000.f;

	FHitResult HitResult;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult,RayStart, RayEnd,ECC_Visibility, Params);

	if (bHit)
	{
		AActor* HitActor = HitResult.GetActor();

		if (HitActor)
		{
			if(HitActor->IsA(AAOfficer::StaticClass()))
			{
				AAOfficer* Officer = Cast<AAOfficer>(HitActor);
				if(Officer)
				{
					if (CurrentOfficer && CurrentOfficer != Officer)
					{
						CurrentOfficer->HideInteractionButton();
					}
					CurrentOfficer = Officer;
					Officer->ShowInteractionButton();
					if (!bIsEventBound)
					{
						Officer->OnInteractionButtonClickedEvent().AddUObject(this,&APacManVRCharacter::StartInteraction);
						Officer->OnEndInteractionButtonClickedEvent().AddUObject(this,&APacManVRCharacter::EndInteraction);
						bIsEventBound = true;
					}
				}
			}
			else if (CurrentOfficer)
			{
				CurrentOfficer->HideInteractionButton();
				CurrentOfficer=nullptr;

			}
		}
		else if (CurrentOfficer)
		{
			CurrentOfficer->HideInteractionButton();
			CurrentOfficer = nullptr;
		}
	}
	
}

void APacManVRCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	// UE_LOG(LogTemp, Warning, TEXT("Tick function is called"));
	PerformRaycast();
}

//////////////////////////////////////////////////////////////////////////// Input

void APacManVRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APacManVRCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APacManVRCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void APacManVRCharacter::ChangeCameraTarget(AAOfficer* Officer)
{
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		float Distance = FVector::Dist(Officer->GetActorLocation(), GetActorLocation());
		UE_LOG(LogTemplateCharacter, Warning, TEXT("Distance: %f"), Distance);
		if (Distance > 160.f)
		{
			if (PC->PlayerCameraManager)
			{
				PC->PlayerCameraManager->SetFOV(30.0f);
			}
		}
		FRotator NewCameraRotation = (Officer->GetActorLocation() - GetActorLocation()).Rotation();
		PC->SetControlRotation(NewCameraRotation);
	}
}

void APacManVRCharacter::StartInteraction(AAOfficer* Officer)
{
	ChangeCameraTarget(Officer);

	EnterInteractionState();
	
	//播放语音
	Officer->StartTalking();
	
}

void APacManVRCharacter::EndInteraction(AAOfficer* Officer)
{
	ExitInteractionState();

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		//Reset Camera rotation
		PC->SetControlRotation(GetActorRotation());
		if (PC->PlayerCameraManager)
		{
			PC->PlayerCameraManager->SetFOV(90.f);
		}
	}
}


void APacManVRCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void APacManVRCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}





