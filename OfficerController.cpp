// Fill out your copyright notice in the Description page of Project Settings.


#include "OfficerController.h"


AOfficerController::AOfficerController()
{
	// 初始化Officer的引用
	ControlledOfficer = nullptr;
}

void AOfficerController::ControlOfficer(FString DialogueName)
{
	ControlledOfficer->DialogueManagerComponent->StartInteraction();
}
