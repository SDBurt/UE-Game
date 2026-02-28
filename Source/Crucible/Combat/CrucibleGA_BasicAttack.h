// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CrucibleGameplayAbility.h"
#include "CrucibleGA_BasicAttack.generated.h"

class UAnimMontage;
class UGameplayEffect;

/**
 * Basic melee attack ability.
 * Plays a montage, performs a sphere trace on anim notify, and applies a damage GE.
 */
UCLASS()
class UCrucibleGA_BasicAttack : public UCrucibleGameplayAbility
{
	GENERATED_BODY()

public:

	UCrucibleGA_BasicAttack();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:

	/** Attack animation montage */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	TObjectPtr<UAnimMontage> AttackMontage;

	/** Gameplay effect applied on hit (should use CrucibleDamageExecution) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	/** Radius of the sphere trace for hit detection */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float TraceRadius = 100.f;

	/** Forward distance for the sphere trace */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float TraceDistance = 150.f;

	/** Perform the sphere trace and apply damage to hit targets */
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void PerformAttackTrace();

private:

	UFUNCTION()
	void OnMontageCompleted(UAnimMontage* Montage, bool bInterrupted);
};
