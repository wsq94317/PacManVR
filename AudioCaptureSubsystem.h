#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Tickable.h"
#include "AudioCaptureCore.h"

#include "AOfficer.h"

#include "AudioCaptureSubsystem.generated.h"


UCLASS()
class PACMANVR_API UAudioCaptureSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

public:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	virtual bool IsTickable() const override;

public:
	UFUNCTION(BlueprintCallable, Category = "AudioCapture")
	void StartCapturingAudio();

	UFUNCTION(BlueprintCallable, Category = "AudioCapture")
	void StopCapturingAudio();

	UFUNCTION(BlueprintCallable, Category = "AudioCapture")
	bool IsCapturingAudio();

	void SetOfficer(AAOfficer* InOfficer);
	

private:
	void SaveAudioToFile();
	// void OnAudioCapture(const float* InAudio, int32 NumSamples);
	void OnAudioCapture(const void* InAudio, int32 NumFrames, int32 InNumChannels, int32 InSampleRate, double StreamTime, bool bOverFlow);

	void GetNewFilePathAndDownloadAudio();
	void DownloadAudioFile(const FString& FileUrl, const FString& Filename);
	// void ImportAndPlaySoundWave(const FString& FilePath, const FString& AssetPath); 

	void PlayDownloadedAudio(const FString& FilePath);
private:
	TArray<int16> CapturedAudioData;
	Audio::FAudioCapture AudioCapture;
	Audio::FAudioCaptureDeviceParams Params;
	AAOfficer* Officer;
	bool bIsCapturing = false; 

};