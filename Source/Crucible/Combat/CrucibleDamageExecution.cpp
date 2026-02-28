// Copyright Epic Games, Inc. All Rights Reserved.

#include "CrucibleDamageExecution.h"
#include "CrucibleAttributeSet.h"
#include "AbilitySystemComponent.h"

struct FCrucibleDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(IncomingDamage);

	FCrucibleDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCrucibleAttributeSet, IncomingDamage, Target, false);
	}
};

static const FCrucibleDamageStatics& DamageStatics()
{
	static FCrucibleDamageStatics Statics;
	return Statics;
}

UCrucibleDamageExecution::UCrucibleDamageExecution()
{
	RelevantAttributesToCapture.Add(DamageStatics().IncomingDamageDef);
}

void UCrucibleDamageExecution::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	if (!SourceASC || !TargetASC)
	{
		return;
	}

	// I-frame check: skip damage if target is dodging
	const FGameplayTag DodgingTag = FGameplayTag::RequestGameplayTag(FName("State.Dodging"), false);
	if (DodgingTag.IsValid() && TargetASC->HasMatchingGameplayTag(DodgingTag))
	{
		return; // No damage output — target is in i-frame
	}

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	// Base damage — for milestone 1, use a flat value.
	// Later: pull from SetByCaller magnitude or source attributes.
	float BaseDamage = 10.f;

	// Check for SetByCaller damage magnitude
	const float SetByCallerDamage = Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Damage.Physical")), false, 0.f);
	if (SetByCallerDamage > 0.f)
	{
		BaseDamage = SetByCallerDamage;
	}

	// Write final damage to the IncomingDamage meta attribute
	if (BaseDamage > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				DamageStatics().IncomingDamageProperty,
				EGameplayModOp::Override,
				BaseDamage
			)
		);
	}
}
