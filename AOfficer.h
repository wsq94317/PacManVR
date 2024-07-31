// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueManagerComponent.h"
#include "GameFramework/Character.h"
#include "UMG.h"
#include "InteractionWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/AudioComponent.h"
class APacManVRCharacter;
class UInteractionWidget; 

#include "AOfficer.generated.h"

UCLASS()
class PACMANVR_API AAOfficer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAOfficer();

	DECLARE_EVENT_OneParam(AAOfficer, FInteractionButtonClickedEvent,AAOfficer*)
	FInteractionButtonClickedEvent& OnInteractionButtonClickedEvent(){return InteractionButtonClickedEvent;}

	DECLARE_EVENT_OneParam(AAOfficer, FEndInteractionButtonClickedEvent, AAOfficer*)
	FEndInteractionButtonClickedEvent& OnEndInteractionButtonClickedEvent(){return EndInteractionButtonClickedEvent;}

public:
	UPROPERTY(BlueprintType,EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> InteractionWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio", meta=(AllowPrivateAccess = "true"))
	UAudioComponent* AudioComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	virtual void ShowInteractionButton();
	void HideInteractionButton();

	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable)
	void OnInteractionButtonClicked();

	UFUNCTION(BlueprintCallable)
	void OnEndInteractionButtonClicked();

	void StartTalking();
	
	UFUNCTION()
	void OnAudioFinished();
	
	void StartRecording();
	
	void EndRecording();

	UAudioComponent* GetAudioComponent() const;

	void OnFinishedOneRoundDiscussion();
	void RefreshDefaultSound();

public:
	UPROPERTY()
	UDialogueManagerComponent* DialogueManagerComponent;


public:
	UPROPERTY(BlueprintReadWrite, Category="Animation")
	bool bIsTalking;
	
private:
	UInteractionWidget* CurrentInteractionWidget;
	bool bIsFirstDialogue;
	
	FInteractionButtonClickedEvent InteractionButtonClickedEvent;

	FEndInteractionButtonClickedEvent EndInteractionButtonClickedEvent;

	UPROPERTY()
	USoundBase* GreetingsSound;


};
