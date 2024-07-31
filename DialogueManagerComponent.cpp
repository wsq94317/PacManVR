// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueManagerComponent.h"

// Sets default values for this component's properties
UDialogueManagerComponent::UDialogueManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	bIsInteracting = false;
	bIsVideoRecording = false;
	bIsVoiceRecording = false;
}


// Called when the game starts
void UDialogueManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UDialogueManagerComponent::StartInteraction()
{
	bIsInteracting = true;
}

void UDialogueManagerComponent::EndInteraction()
{
	bIsInteracting = false;
}

void UDialogueManagerComponent::StartVoiceRecording()
{
	bIsVoiceRecording = true;
}

void UDialogueManagerComponent::EndVoiceRecording()
{
	bIsVoiceRecording = false;
}

void UDialogueManagerComponent::StartVideoRecording()
{
	bIsVideoRecording = true;
}

void UDialogueManagerComponent::EndVideoRecording()
{
	bIsVideoRecording = false;
}

void UDialogueManagerComponent::SendRecordingToServer()
{
}


void UDialogueManagerComponent::ReceiveRecordingFromServer()
{
}

// Called every frame
void UDialogueManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

