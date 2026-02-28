// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "CrucibleDamageExecution.generated.h"

/**
 * Execution calculation for the Crucible damage pipeline.
 * Calculates final damage and writes it to the IncomingDamage meta attribute.
 */
UCLASS()
class CRUCIBLE_API UCrucibleDamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:

	UCrucibleDamageExecution();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
