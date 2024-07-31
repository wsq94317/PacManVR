// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Logging/LogMacros.h"

class AAOfficer;

#include "PacManVRCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	Free,
	Interacting
};

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class APacManVRCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	
public:
	APacManVRCharacter();

public:
	UPROPERTY(BlueprintReadOnly, Category = "State")
	EPlayerState CurrentState;

	UFUNCTION(BlueprintCallable, Category = "State")
	void EnterInteractionState();

	UFUNCTION(BlueprintCallable, Category = "State")
	void ExitInteractionState();	


protected:
	virtual void BeginPlay();

	void PerformRaycast();

	void Tick(float DeltaSeconds) override;

private:
	AAOfficer* CurrentOfficer;
	bool bIsEventBound;

public:
		
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }



public:

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void ChangeCameraTarget(AAOfficer* Officer);

	
	void StartInteraction(AAOfficer* Officer);

	void EndInteraction(AAOfficer* Officer);
	
};

