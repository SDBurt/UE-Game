// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CrucibleGameplayAbility.generated.h"

/**
 * Base gameplay ability for Crucible.
 * Adds an AbilityInputTag so abilities can be mapped to input via gameplay tags.
 */
UCLASS(Abstract)
class UCrucibleGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UCrucibleGameplayAbility();

	/** Tag name used to map this ability to an input action (resolved at runtime) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	FName AbilityTagName;

	/** Resolve the tag at runtime (after tags are registered) */
	FGameplayTag GetAbilityTag() const;
};
