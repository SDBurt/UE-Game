// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "CrucibleTestPawn.generated.h"

class UCrucibleAbilitySystemComponent;
class UCrucibleAttributeSet;

/**
 * Minimal actor with ASC + AttributeSet for automated tests.
 * Uses a plain AActor instead of ACharacter to avoid capsule/movement overhead.
 */
UCLASS()
class ACrucibleTestPawn : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	ACrucibleTestPawn();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UCrucibleAttributeSet* GetAttributeSet() const { return AttributeSet; }

protected:

	UPROPERTY()
	TObjectPtr<UCrucibleAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UCrucibleAttributeSet> AttributeSet;
};
