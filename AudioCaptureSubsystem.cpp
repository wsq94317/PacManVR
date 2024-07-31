#include "AudioCaptureSubsystem.h"
#include "HttpModule.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "TimerManager.h"
#include "Interfaces/IAudioFormat.h"
#include "Sound/SoundWave.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundWaveProcedural.h"

bool UAudioCaptureSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
    return Super::ShouldCreateSubsystem(Outer);
}

void UAudioCaptureSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    Params.DeviceIndex = INDEX_NONE; // Use default device

    AudioCapture.OpenAudioCaptureStream(
       Params,
       [this](const void* InAudio, int32 NumFrames, int32 InNumChannels, int32 InSampleRate, double StreamTime, bool bOverFlow)
       {
           OnAudioCapture(InAudio, NumFrames, InNumChannels, InSampleRate, StreamTime, bOverFlow);
       },
       1024
   );
}

void UAudioCaptureSubsystem::Deinitialize()
{
    if (bIsCapturing)
    {
        StopCapturingAudio();
    }
    AudioCapture.AbortStream();
    Super::Deinitialize();
}

void UAudioCaptureSubsystem::Tick(float DeltaTime)
{
}

TStatId UAudioCaptureSubsystem::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(UAudioCaptureSubsystem, STATGROUP_Tickables);
}

bool UAudioCaptureSubsystem::IsTickable() const
{
    return FTickableGameObject::IsTickable();
}

void UAudioCaptureSubsystem::StartCapturingAudio()
{
    if (AudioCapture.IsCapturing())
    {
        AudioCapture.StopStream();
        UE_LOG(LogTemp, Display, TEXT("Audio capture stopped"));
    }
    
    if (!AudioCapture.IsStreamOpen())
    {
        AudioCapture.OpenAudioCaptureStream(
            Params,
            [this](const void* InAudio, int32 NumFrames, int32 InNumChannels, int32 InSampleRate, double StreamTime, bool bOverFlow)
            {
                OnAudioCapture(InAudio, NumFrames, InNumChannels, InSampleRate, StreamTime, bOverFlow);
            },
            1024
        );
    }

    CapturedAudioData.Empty();
    if (!AudioCapture.IsCapturing())
    {
        AudioCapture.StartStream();
    }
}

void UAudioCaptureSubsystem::StopCapturingAudio()
{
    if (AudioCapture.IsCapturing())
    {
        AudioCapture.StopStream();
        UE_LOG(LogTemp, Display, TEXT("Audio capture stopped"));
    }
    if (AudioCapture.IsStreamOpen())
    {
        AudioCapture.CloseStream();
        UE_LOG(LogTemp, Display, TEXT("Audio capture stream closed"));
    }
    SaveAudioToFile();
}

bool UAudioCaptureSubsystem::IsCapturingAudio()
{
    return AudioCapture.IsCapturing();
}

void UAudioCaptureSubsystem::SetOfficer(AAOfficer* InOfficer)
{
    Officer = InOfficer;
}

void UAudioCaptureSubsystem::OnAudioCapture(const void* InAudio, int32 NumFrames, int32 InNumChannels, int32 InSampleRate, double StreamTime, bool bOverFlow)
{
    UE_LOG(LogTemp, Display, TEXT("OnAudioCapture: %d frames, %d channels, %d sample rate"), NumFrames, InNumChannels, InSampleRate);

    const float* FloatAudioData = static_cast<const float*>(InAudio);
    TArray<int16> PCMData;
    PCMData.SetNumUninitialized(NumFrames * InNumChannels);

    float maxAmplitude = 0.0f;

    for (int32 SampleIndex = 0; SampleIndex < NumFrames * InNumChannels; ++SampleIndex)
    {
        float normalizedSample = FMath::Clamp(FloatAudioData[SampleIndex] * 32767.0f, -32768.0f, 32767.0f);
        PCMData[SampleIndex] = static_cast<int16>(normalizedSample);

        if (FMath::Abs(normalizedSample) > maxAmplitude)
        {
            maxAmplitude = FMath::Abs(normalizedSample);
        }
    }

    UE_LOG(LogTemp, Display, TEXT("Max amplitude: %f"), maxAmplitude);

    CapturedAudioData.Append(PCMData);
}

void UAudioCaptureSubsystem::SaveAudioToFile()
{
    if (CapturedAudioData.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No audio data to save."));
        return;
    }

    TArray<uint8> WavData;
    int32 NumChannels = 1; // 单声道
    int32 SampleRate = 44100; // 采样率

    WavData.Append((uint8*)"RIFF", 4);
    int32 ChunkSize = 36 + CapturedAudioData.Num() * sizeof(int16);
    WavData.Append((uint8*)&ChunkSize, 4);
    WavData.Append((uint8*)"WAVE", 4);

    WavData.Append((uint8*)"fmt ", 4);
    int32 Subchunk1Size = 16;
    int16 AudioFormat = 1; // PCM
    int16 NumChannels16 = NumChannels;
    int32 SampleRate32 = SampleRate;
    int32 ByteRate = SampleRate * NumChannels * sizeof(int16);
    int16 BlockAlign = NumChannels * sizeof(int16);
    int16 BitsPerSample = sizeof(int16) * 8;
    WavData.Append((uint8*)&Subchunk1Size, 4);
    WavData.Append((uint8*)&AudioFormat, 2);
    WavData.Append((uint8*)&NumChannels16, 2);
    WavData.Append((uint8*)&SampleRate32, 4);
    WavData.Append((uint8*)&ByteRate, 4);
    WavData.Append((uint8*)&BlockAlign, 2);
    WavData.Append((uint8*)&BitsPerSample, 2);

    WavData.Append((uint8*)"data", 4);
    int32 Subchunk2Size = CapturedAudioData.Num() * sizeof(int16);
    WavData.Append((uint8*)&Subchunk2Size, 4);
    WavData.Append(reinterpret_cast<const uint8*>(CapturedAudioData.GetData()), CapturedAudioData.Num() * sizeof(int16));

    FString Boundary = "---------------------------741888291336639302736114993";
    FString NewLine = "\r\n";

    TArray<uint8> FormData;
    FString FormDataString;

    FormDataString.Append("--" + Boundary + NewLine);
    FormDataString.Append("Content-Disposition: form-data; name=\"text\"" + NewLine + NewLine);
    FormDataString.Append("Sample Text");
    FormDataString.Append(NewLine);

    FormDataString.Append("--" + Boundary + NewLine);
    FormDataString.Append("Content-Disposition: form-data; name=\"file\"; filename=\"recorded_audio.wav\"" + NewLine);
    FormDataString.Append("Content-Type: audio/wav" + NewLine + NewLine);

    FormData.Append((uint8*)TCHAR_TO_ANSI(*FormDataString), FormDataString.Len());
    FormData.Append(WavData);
    FormDataString = NewLine + "--" + Boundary + "--" + NewLine;
    FormData.Append((uint8*)TCHAR_TO_ANSI(*FormDataString), FormDataString.Len());

    FHttpModule* Http = &FHttpModule::Get();
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();

    Request->OnProcessRequestComplete().BindLambda([](FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
    {
        if (bSucceeded && HttpResponse.IsValid())
        {
            UE_LOG(LogTemp, Log, TEXT("Upload success: %s"), *HttpResponse->GetContentAsString());
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Upload failed"));
        }
    });

    FString Url = TEXT("http://127.0.0.1:5000/upload");
    Request->SetURL(Url);
    Request->SetVerb("POST");
    Request->SetHeader("Content-Type", "multipart/form-data; boundary=" + Boundary);
    Request->SetContent(FormData);
    Request->ProcessRequest();

    GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
    {
        FTimerHandle TimerHandle;
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UAudioCaptureSubsystem::GetNewFilePathAndDownloadAudio, 5.0f, false);
    });
}

void UAudioCaptureSubsystem::GetNewFilePathAndDownloadAudio()
{
    FHttpModule* Http = &FHttpModule::Get();
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();

    Request->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
    {
        if (bSucceeded && HttpResponse.IsValid())
        {
            FString ResponseStr = HttpResponse->GetContentAsString();
            TSharedPtr<FJsonObject> JsonObject;
            TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseStr);

            if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
            {
                FString RelativeFileUrl = JsonObject->GetStringField("file_url");
                FString Text = JsonObject->GetStringField("text");
                FString Filename = JsonObject->GetStringField("filename");

                FString FileUrl = FString::Printf(TEXT("http://127.0.0.1:5000/%s"), *RelativeFileUrl);
                DownloadAudioFile(FileUrl, Filename);
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON response"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to get latest media info"));
        }
    });

    FString Url = TEXT("http://127.0.0.1:5000/latest_media");
    Request->SetURL(Url);
    Request->SetVerb("GET");
    Request->ProcessRequest();
}

void UAudioCaptureSubsystem::DownloadAudioFile(const FString& FileUrl, const FString& Filename)
{
    FHttpModule* Http = &FHttpModule::Get();
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();

    Request->OnProcessRequestComplete().BindLambda([this, Filename](FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
    {
        if (bSucceeded && HttpResponse.IsValid())
        {
            UE_LOG(LogTemp, Log, TEXT("Download success"));

            FString FilePath = FPaths::ProjectDir() + TEXT("Saved/") + Filename;
            if (FFileHelper::SaveArrayToFile(HttpResponse->GetContent(), *FilePath))
            {
                UE_LOG(LogTemp, Log, TEXT("Successfully saved downloaded audio to file: %s"), *FilePath);

                PlayDownloadedAudio(FilePath);
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to save downloaded audio to file: %s"), *FilePath);
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Download failed"));
        }
    });

    Request->SetURL(FileUrl);
    Request->SetVerb("GET");
    Request->ProcessRequest();
}

void UAudioCaptureSubsystem::PlayDownloadedAudio(const FString& FilePath)
{
    if (!Officer)
    {
        UE_LOG(LogTemp, Error, TEXT("Officer not set"));
        return;
    }

    if (!FPaths::FileExists(FilePath))
    {
        UE_LOG(LogTemp, Error, TEXT("File does not exist: %s"), *FilePath);
        return;
    }

    TArray<uint8> RawFileData;
    if (!FFileHelper::LoadFileToArray(RawFileData, *FilePath))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load file: %s"), *FilePath);
        return;
    }

    FWaveModInfo WaveInfo;
    if (!WaveInfo.ReadWaveInfo(RawFileData.GetData(), RawFileData.Num()))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to read wave info"));
        return;
    }

    USoundWaveProcedural* SoundWave = NewObject<USoundWaveProcedural>();
    if (!SoundWave)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create SoundWaveProcedural"));
        return;
    }

    // 设置SoundWave的属性
    SoundWave->SetSampleRate(*WaveInfo.pSamplesPerSec);
    SoundWave->NumChannels = *WaveInfo.pChannels;

    // 将音频数据传输到SoundWave
    SoundWave->QueueAudio(RawFileData.GetData(), RawFileData.Num());

    // 设置音频持续时间
    SoundWave->Duration = (float)RawFileData.Num() / (*WaveInfo.pSamplesPerSec * *WaveInfo.pChannels * 2); // 2 for 16-bit audio

    // 播放音频
    UAudioComponent* OfficerAudioComponent = Officer->GetAudioComponent();
    if (OfficerAudioComponent)
    {
        OfficerAudioComponent->SetSound(SoundWave);
        OfficerAudioComponent->Play();

        // 绑定音频完成事件
        OfficerAudioComponent->OnAudioFinished.RemoveDynamic(Officer, &AAOfficer::OnAudioFinished);
        OfficerAudioComponent->OnAudioFinished.AddDynamic(Officer, &AAOfficer::OnAudioFinished);

        Officer->bIsTalking = true;

        // Debug 输出当前音频的持续时间
        UE_LOG(LogTemp, Display, TEXT("Playing sound with duration: %f seconds"), SoundWave->Duration);
        // 手动触发音频完成事件 Hard code
        FTimerHandle TimerHandle;
        Officer->GetWorld()->GetTimerManager().SetTimer(TimerHandle, [Officer = Officer]()
        {
            if (Officer && Officer->GetAudioComponent())
            {
                Officer->OnAudioFinished();
                Officer->OnFinishedOneRoundDiscussion();
            }
        }, SoundWave->Duration, false);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Officer's AudioComponent not found"));
    }
}