// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CrucibleGameMode.generated.h"

/**
 * Base GameMode for Crucible.
 * Sets default pawn and controller classes.
 * Subclass in Blueprint (BP_CrucibleGameMode) to assign specific assets.
 */
UCLASS()
class ACrucibleGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	ACrucibleGameMode();
};
