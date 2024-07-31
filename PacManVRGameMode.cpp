// Copyright Epic Games, Inc. All Rights Reserved.

#include "PacManVRGameMode.h"
#include "PacManVRCharacter.h"
#include "UObject/ConstructorHelpers.h"

APacManVRGameMode::APacManVRGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
