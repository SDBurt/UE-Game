// Copyright Epic Games, Inc. All Rights Reserved.

#include "CrucibleGameplayTags.h"
#include "GameplayTagsManager.h"

FCrucibleGameplayTags FCrucibleGameplayTags::GameplayTags;

void FCrucibleGameplayTags::InitializeNativeTags()
{
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.BasicAttack"), FString("Basic melee attack"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Dodge"), FString("Dodge / i-frame roll"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Skill1"), FString("Skill slot 1"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Skill2"), FString("Skill slot 2"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Skill3"), FString("Skill slot 3"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Skill4"), FString("Skill slot 4"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Ultimate"), FString("Ultimate ability"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.ChainAttack"), FString("Party chain attack"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("State.Dead"), FString("Character is dead"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("State.Stunned"), FString("Character is stunned"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("State.Attacking"), FString("Character is performing an attack"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("State.Dodging"), FString("Character is dodging"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("State.Sprinting"), FString("Character is sprinting"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Element.Heat"), FString("Heat / Fire element"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Element.Electric"), FString("Electric / Lightning element"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Element.Cryo"), FString("Cryo / Ice element"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Element.Nature"), FString("Nature / Poison element"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Reaction.Combustion"), FString("Heat + Nature reaction"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Reaction.Electrification"), FString("Electric + Cryo reaction"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Reaction.Corrosion"), FString("Nature + Electric reaction"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Reaction.Solidification"), FString("Cryo + Heat reaction"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Reaction.Shatter"), FString("Physical hit on frozen target"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Physical"), FString("Physical damage type"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Elemental"), FString("Elemental damage type"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Status.Staggered"), FString("Target is staggered (stagger bar full)"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Status.Burning"), FString("Burning DoT"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Status.Frozen"), FString("Frozen / immobilized"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Status.Shocked"), FString("Shocked / intermittent stun"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Status.Poisoned"), FString("Poisoned DoT"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Damage"), FString("Damage event for gameplay cues"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.ChainOpportunity"), FString("Chain attack opportunity window"));

	// Now resolve all tags into the singleton for runtime access
	GameplayTags.ResolveAllTags();
}

void FCrucibleGameplayTags::ResolveAllTags()
{
	Ability_BasicAttack = FGameplayTag::RequestGameplayTag(FName("Ability.BasicAttack"));
	Ability_Dodge = FGameplayTag::RequestGameplayTag(FName("Ability.Dodge"));
	Ability_Skill1 = FGameplayTag::RequestGameplayTag(FName("Ability.Skill1"));
	Ability_Skill2 = FGameplayTag::RequestGameplayTag(FName("Ability.Skill2"));
	Ability_Skill3 = FGameplayTag::RequestGameplayTag(FName("Ability.Skill3"));
	Ability_Skill4 = FGameplayTag::RequestGameplayTag(FName("Ability.Skill4"));
	Ability_Ultimate = FGameplayTag::RequestGameplayTag(FName("Ability.Ultimate"));
	Ability_ChainAttack = FGameplayTag::RequestGameplayTag(FName("Ability.ChainAttack"));
	State_Dead = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
	State_Stunned = FGameplayTag::RequestGameplayTag(FName("State.Stunned"));
	State_Attacking = FGameplayTag::RequestGameplayTag(FName("State.Attacking"));
	State_Dodging = FGameplayTag::RequestGameplayTag(FName("State.Dodging"));
	State_Sprinting = FGameplayTag::RequestGameplayTag(FName("State.Sprinting"));
	Element_Heat = FGameplayTag::RequestGameplayTag(FName("Element.Heat"));
	Element_Electric = FGameplayTag::RequestGameplayTag(FName("Element.Electric"));
	Element_Cryo = FGameplayTag::RequestGameplayTag(FName("Element.Cryo"));
	Element_Nature = FGameplayTag::RequestGameplayTag(FName("Element.Nature"));
	Reaction_Combustion = FGameplayTag::RequestGameplayTag(FName("Reaction.Combustion"));
	Reaction_Electrification = FGameplayTag::RequestGameplayTag(FName("Reaction.Electrification"));
	Reaction_Corrosion = FGameplayTag::RequestGameplayTag(FName("Reaction.Corrosion"));
	Reaction_Solidification = FGameplayTag::RequestGameplayTag(FName("Reaction.Solidification"));
	Reaction_Shatter = FGameplayTag::RequestGameplayTag(FName("Reaction.Shatter"));
	Damage_Physical = FGameplayTag::RequestGameplayTag(FName("Damage.Physical"));
	Damage_Elemental = FGameplayTag::RequestGameplayTag(FName("Damage.Elemental"));
	Status_Staggered = FGameplayTag::RequestGameplayTag(FName("Status.Staggered"));
	Status_Burning = FGameplayTag::RequestGameplayTag(FName("Status.Burning"));
	Status_Frozen = FGameplayTag::RequestGameplayTag(FName("Status.Frozen"));
	Status_Shocked = FGameplayTag::RequestGameplayTag(FName("Status.Shocked"));
	Status_Poisoned = FGameplayTag::RequestGameplayTag(FName("Status.Poisoned"));
	Event_Damage = FGameplayTag::RequestGameplayTag(FName("Event.Damage"));
	Event_ChainOpportunity = FGameplayTag::RequestGameplayTag(FName("Event.ChainOpportunity"));
}
