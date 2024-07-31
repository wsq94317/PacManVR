// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DialogueManagerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PACMANVR_API UDialogueManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDialogueManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category= "Dialogue")
	void StartInteraction();

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void EndInteraction();

	UFUNCTION(BlueprintCallable, Category="Dialogue")
	void StartVoiceRecording();

	UFUNCTION(BlueprintCallable, Category="Dialogue")
	void EndVoiceRecording();

	UFUNCTION(BlueprintCallable, Category="Dialogue")
	void StartVideoRecording();

	UFUNCTION(BlueprintCallable, Category="Dialogue")
	void EndVideoRecording();

	UFUNCTION(BlueprintCallable, Category="Dialogue")
	void SendRecordingToServer();

	UFUNCTION(BlueprintCallable, Category="Dialogue")
	void ReceiveRecordingFromServer();
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	bool bIsInteracting;

	UPROPERTY()
	bool bIsVoiceRecording;

	UPROPERTY()
	bool bIsVideoRecording;

	UPROPERTY()
	USoundWave* Recording;

	UPROPERTY()
	USoundWave* ServerResponseAudio;	
};
