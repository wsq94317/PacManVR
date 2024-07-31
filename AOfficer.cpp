#include "AOfficer.h"
#include "AudioCaptureSubsystem.h"
#include "InteractionWidget.h"
#include "PacManVRCharacter.h"

// Sets default values
AAOfficer::AAOfficer()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    bIsFirstDialogue = false;
    CurrentInteractionWidget = nullptr;
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Block);
    GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECollisionResponse::ECR_Block);
    GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECollisionResponse::ECR_Block);

    DialogueManagerComponent = CreateDefaultSubobject<UDialogueManagerComponent>(TEXT("DialogueManagerComponent"));

    //Generate AudioComponent
    AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
    AudioComponent->bAutoActivate = false;

    static ConstructorHelpers::FObjectFinder<USoundBase> Sound(TEXT("/Game/Voice/Greetings.Greetings"));
    if (Sound.Succeeded())
    {
        GreetingsSound = Sound.Object;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load Greetings sound"));
    }


    bIsTalking = false;
}

// Called when the game starts or when spawned
void AAOfficer::BeginPlay()
{
    Super::BeginPlay();

    UAudioCaptureSubsystem* AudioCaptureSubsystem = GetGameInstance()->GetSubsystem<UAudioCaptureSubsystem>();
    if (AudioCaptureSubsystem)
    {
        AudioCaptureSubsystem->SetOfficer(this);
    }
}

// Called every frame
void AAOfficer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AAOfficer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AAOfficer::ShowInteractionButton()
{
    if (InteractionWidgetClass && !CurrentInteractionWidget)
    {
        CurrentInteractionWidget = CreateWidget<UInteractionWidget>(GetWorld(), InteractionWidgetClass);
        if (CurrentInteractionWidget)
        {
            UInteractionWidget* InteractionWidget = Cast<UInteractionWidget>(CurrentInteractionWidget);
            if (InteractionWidget)
            {
                InteractionWidget->Officer = this;
            }
            CurrentInteractionWidget->AddToViewport();
        }
    }
}

void AAOfficer::HideInteractionButton()
{
    if (CurrentInteractionWidget)
    {
        CurrentInteractionWidget->RemoveFromViewport();
        CurrentInteractionWidget = nullptr;
    }
}

void AAOfficer::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    UE_LOG(LogTemp, Warning, TEXT("AAOfficer is being destroyed, reason: %d"), EndPlayReason);
}

void AAOfficer::OnInteractionButtonClicked()
{
    InteractionButtonClickedEvent.Broadcast(this);
}

void AAOfficer::OnEndInteractionButtonClicked()
{
    EndInteractionButtonClickedEvent.Broadcast(this);
}

void AAOfficer::StartTalking()
{
    if (AudioComponent )
    {
        RefreshDefaultSound();
        AudioComponent->Play();
        bIsTalking = true;
        AudioComponent->OnAudioFinished.RemoveDynamic(this, &AAOfficer::OnAudioFinished);
        AudioComponent->OnAudioFinished.AddDynamic(this, &AAOfficer::OnAudioFinished);
    }
}

void AAOfficer::OnAudioFinished()
{
    if (AudioComponent)
    {
        AudioComponent->Stop();
        bIsTalking = false;
    }
}

void AAOfficer::StartRecording()
{
    UAudioCaptureSubsystem* AudioCaptureSubsystem = GetGameInstance()->GetSubsystem<UAudioCaptureSubsystem>();
    if (AudioCaptureSubsystem)
    {
        AudioCaptureSubsystem->StartCapturingAudio();
    }
}

void AAOfficer::EndRecording()
{
    UAudioCaptureSubsystem* AudioCaptureSubsystem = GetGameInstance()->GetSubsystem<UAudioCaptureSubsystem>();
    if (AudioCaptureSubsystem)
    {
        AudioCaptureSubsystem->StopCapturingAudio();
    }
}

UAudioComponent* AAOfficer::GetAudioComponent() const
{
    return AudioComponent;
}

void AAOfficer::OnFinishedOneRoundDiscussion()
{
    if (CurrentInteractionWidget)
    {
        CurrentInteractionWidget->SwitchInteractionMethodButtonsVisibility(true);
    }
}

void AAOfficer::RefreshDefaultSound()
{
    AudioComponent->SetSound(GreetingsSound);
}



