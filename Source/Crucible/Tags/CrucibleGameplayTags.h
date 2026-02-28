// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * Singleton struct containing native gameplay tags for the Crucible project.
 * Tags are registered in C++ and always available — no data table needed.
 */
struct CRUCIBLE_API FCrucibleGameplayTags
{
public:

	static const FCrucibleGameplayTags& Get() { return GameplayTags; }

	static void InitializeNativeTags();

	// --- Ability ---
	FGameplayTag Ability_BasicAttack;
	FGameplayTag Ability_Dodge;
	FGameplayTag Ability_Skill1;
	FGameplayTag Ability_Skill2;
	FGameplayTag Ability_Skill3;
	FGameplayTag Ability_Skill4;
	FGameplayTag Ability_Ultimate;
	FGameplayTag Ability_ChainAttack;

	// --- State ---
	FGameplayTag State_Dead;
	FGameplayTag State_Stunned;
	FGameplayTag State_Attacking;
	FGameplayTag State_Dodging;
	FGameplayTag State_Sprinting;

	// --- Element ---
	FGameplayTag Element_Heat;
	FGameplayTag Element_Electric;
	FGameplayTag Element_Cryo;
	FGameplayTag Element_Nature;

	// --- Reaction ---
	FGameplayTag Reaction_Combustion;
	FGameplayTag Reaction_Electrification;
	FGameplayTag Reaction_Corrosion;
	FGameplayTag Reaction_Solidification;
	FGameplayTag Reaction_Shatter;

	// --- Damage ---
	FGameplayTag Damage_Physical;
	FGameplayTag Damage_Elemental;

	// --- Status ---
	FGameplayTag Status_Staggered;
	FGameplayTag Status_Burning;
	FGameplayTag Status_Frozen;
	FGameplayTag Status_Shocked;
	FGameplayTag Status_Poisoned;

	// --- Event ---
	FGameplayTag Event_Damage;
	FGameplayTag Event_ChainOpportunity;

private:

	static FCrucibleGameplayTags GameplayTags;

	void ResolveAllTags();
};
