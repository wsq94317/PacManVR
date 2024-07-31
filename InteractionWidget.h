// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
class AAOfficer; 
#include "InteractionWidget.generated.h"

/**
 * 
 */


UENUM(BlueprintType)
enum class ECommunicateState : uint8
{
	PrepareInteract,
	VoiceRecording,
	VideoRecording,
	WaitingForResponse,
	PlayResponse,
	Free,
};

UCLASS()
class PACMANVR_API UInteractionWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	AAOfficer* Officer;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnVoiceMessageButtonClicked();
	
	UFUNCTION()
	void OnVideoMessageButtonCLicked();

	UFUNCTION()
	void OnSendVoiceButtonClicked();
	
	UFUNCTION()
	void SwitchInteractionMethodButtonsVisibility(bool bIsVisible);
	
	UFUNCTION()
	void OnSendVideoButtonClicked();

	UFUNCTION()
	void OnInteractionButtonClicked();

	UFUNCTION()
	void OnEndInteractionButtonClicked();

	UFUNCTION()
	void OnSendTextButtonClicked();

	UFUNCTION()
	void SwitchInteractingButtonVisibility(bool bIsInteracting);

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void ShowDevelopingMessage();

public:
	UPROPERTY()
	ECommunicateState CommunicateState;

	FTimerHandle MessageTimerHandle;
	bool bIsMessageShowing;
    
	void HideDevelopingMessage();

};
