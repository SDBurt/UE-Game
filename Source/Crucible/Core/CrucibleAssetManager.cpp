// Copyright Epic Games, Inc. All Rights Reserved.

#include "CrucibleAssetManager.h"
#include "CrucibleGameplayTags.h"

void UCrucibleAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FCrucibleGameplayTags::InitializeNativeTags();
}
