// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AOfficer.h"
#include "OfficerController.generated.h"

/**
 * 
 */
UCLASS()
class PACMANVR_API AOfficerController : public AAIController
{
	GENERATED_BODY()

public:
	AOfficerController();

	void ControlOfficer(FString DialogueName);

protected:
	AAOfficer* ControlledOfficer;

	
};
