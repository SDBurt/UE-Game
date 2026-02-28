// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CrucibleAttributeSet.generated.h"

/**
 * Macro that defines boilerplate accessors for a gameplay attribute.
 * UE does not ship this macro — we define it ourselves.
 */
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * Core attribute set for Crucible characters.
 * Contains Health, SkillPoints, StaggerDamage, and the IncomingDamage meta attribute.
 */
UCLASS()
class CRUCIBLE_API UCrucibleAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	UCrucibleAttributeSet();

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	// --- Health ---
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Health")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UCrucibleAttributeSet, Health)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Health")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UCrucibleAttributeSet, MaxHealth)

	// --- Skill Points ---
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|SkillPoints")
	FGameplayAttributeData SkillPoints;
	ATTRIBUTE_ACCESSORS(UCrucibleAttributeSet, SkillPoints)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|SkillPoints")
	FGameplayAttributeData MaxSkillPoints;
	ATTRIBUTE_ACCESSORS(UCrucibleAttributeSet, MaxSkillPoints)

	// --- Stagger ---
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Stagger")
	FGameplayAttributeData StaggerDamage;
	ATTRIBUTE_ACCESSORS(UCrucibleAttributeSet, StaggerDamage)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Stagger")
	FGameplayAttributeData MaxStaggerDamage;
	ATTRIBUTE_ACCESSORS(UCrucibleAttributeSet, MaxStaggerDamage)

	// --- Meta (not replicated, consumed immediately) ---
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Meta")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UCrucibleAttributeSet, IncomingDamage)
};
