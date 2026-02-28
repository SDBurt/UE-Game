// Copyright Epic Games, Inc. All Rights Reserved.

#include "CrucibleGameMode.h"
#include "CruciblePlayerController.h"
#include "CruciblePlayerCharacter.h"

ACrucibleGameMode::ACrucibleGameMode()
{
	DefaultPawnClass = ACruciblePlayerCharacter::StaticClass();
	PlayerControllerClass = ACruciblePlayerController::StaticClass();
}
