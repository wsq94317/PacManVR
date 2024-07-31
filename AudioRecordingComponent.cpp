#include "AudioRecordingComponent.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"


// Sets default values for this component's properties
UAudioRecordingComponent::UAudioRecordingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UAudioRecordingComponent::BeginPlay()
{
	Super::BeginPlay();
}

