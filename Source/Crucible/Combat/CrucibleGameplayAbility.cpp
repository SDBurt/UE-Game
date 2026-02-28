// Copyright Epic Games, Inc. All Rights Reserved.

#include "CrucibleGameplayAbility.h"

UCrucibleGameplayAbility::UCrucibleGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

FGameplayTag UCrucibleGameplayAbility::GetAbilityTag() const
{
	if (AbilityTagName.IsNone())
	{
		return FGameplayTag();
	}
	return FGameplayTag::RequestGameplayTag(AbilityTagName, false);
}
