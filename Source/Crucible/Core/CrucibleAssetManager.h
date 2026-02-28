// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "CrucibleAssetManager.generated.h"

/**
 * Custom AssetManager that initializes native gameplay tags on startup.
 * Must be set in DefaultEngine.ini: AssetManagerClassName=/Script/Crucible.CrucibleAssetManager
 */
UCLASS()
class UCrucibleAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:

	virtual void StartInitialLoading() override;
};
