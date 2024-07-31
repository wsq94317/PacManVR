#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "AudioRecordingComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PACMANVR_API UAudioRecordingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAudioRecordingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
};