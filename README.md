# PacManVR

## Project Overview

This project was designed for Westpac Bank to create a virtual reality banking environment. It allows users to perform banking operations within a virtual bank setting, interacting with AI bank employees driven by a generative language model. Users can input voice commands and receive voice responses, with future expansion capabilities for text input, sign language recognition, and eye-tracking interfaces.

## Project Achievements

- Winner of the 2024 Westpac Hackathon Coding Competition.
- Project demonstration video: [Watch on YouTube](https://www.youtube.com/watch?v=bPV44ev2RU8)

## Project Structure

- **Source Files:**
  - `AOfficer.cpp`, `AOfficer.h`: Defines the behavior and properties of an officer NPC.
  - `AudioCaptureSubsystem.cpp`, `AudioCaptureSubsystem.h`: Manages audio capture functionalities.
  - `AudioHelper.cpp`, `AudioHelper.h`: Provides utility functions for audio management.
  - `AudioRecordingComponent.cpp`, `AudioRecordingComponent.h`: Handles audio recording operations.
  - `DialogueManagerComponent.cpp`, `DialogueManagerComponent.h`: Manages dialogue interactions.
  - `InteractionWidget.cpp`, `InteractionWidget.h`: Defines the interaction widget for the VR interface.
  - `OfficerController.cpp`, `OfficerController.h`: Controls the officer NPC.
  - `PacManVR.Build.cs`: Configures the build settings for the PacManVR project.
  - `PacManVR.cpp`, `PacManVR.h`: Main game module implementation.
  - `PacManVRCharacter.cpp`, `PacManVRCharacter.h`: Defines the player's VR character.
  - `PacManVRGameMode.cpp`, `PacManVRGameMode.h`: Configures the game mode settings.
  - `PacManVR.Target.cs`, `PacManVREditor.Target.cs`: Specifies the build targets for the project.

## Prerequisites

- Unreal Engine 5.4.2
- Microsoft Visual Studio 2022
- Windows 10 SDK version 10.0.19041.0

