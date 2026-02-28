// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "CrucibleCharacterBase.generated.h"

class UCrucibleAbilitySystemComponent;
class UCrucibleAttributeSet;
class UGameplayAbility;
class UGameplayEffect;

/**
 * Base character class for all Crucible characters.
 * Owns an AbilitySystemComponent and AttributeSet (Pattern A — ASC on Character).
 */
UCLASS(Abstract)
class ACrucibleCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	ACrucibleCharacterBase();

	// -- IAbilitySystemInterface --
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UCrucibleAttributeSet* GetAttributeSet() const { return AttributeSet; }

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UCrucibleAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UCrucibleAttributeSet> AttributeSet;

	/** Abilities granted on spawn */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|Defaults")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

	/** Effect applied on spawn to initialize attributes */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|Defaults")
	TSubclassOf<UGameplayEffect> DefaultAttributeEffect;

	virtual void BeginPlay() override;

	/** Grant default abilities to this character */
	void GiveDefaultAbilities();

	/** Apply the default attribute initialization effect */
	void ApplyDefaultAttributes();
};
