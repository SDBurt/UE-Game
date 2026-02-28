// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "CrucibleAbilitySystemComponent.generated.h"

/**
 * Thin AbilitySystemComponent subclass for Crucible.
 * Provides a project-level extension point without modifying engine code.
 */
UCLASS()
class CRUCIBLE_API UCrucibleAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:

	UCrucibleAbilitySystemComponent();
};
