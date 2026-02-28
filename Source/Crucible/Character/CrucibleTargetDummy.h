// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CrucibleCharacterBase.h"
#include "CrucibleTargetDummy.generated.h"

/**
 * Target dummy for testing the damage pipeline.
 * Inherits CharacterBase (gets ASC + AttributeSet automatically).
 * Override OnDamageTaken in Blueprint to show floating damage numbers, etc.
 */
UCLASS()
class ACrucibleTargetDummy : public ACrucibleCharacterBase
{
	GENERATED_BODY()

public:

	ACrucibleTargetDummy();

	/** Blueprint event fired when this dummy takes damage */
	UFUNCTION(BlueprintImplementableEvent, Category = "Damage")
	void OnDamageTaken(float DamageAmount, AActor* DamageSource);

protected:

	virtual void BeginPlay() override;
};
