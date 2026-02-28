// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CrucibleGameplayAbility.h"
#include "CrucibleGA_Dodge.generated.h"

class UAnimMontage;

/**
 * Directional dodge ability with i-frame invulnerability.
 * Dodges in the direction of WASD input (or backward if no input).
 * Grants State.Dodging tag during i-frame window to block damage.
 */
UCLASS()
class UCrucibleGA_Dodge : public UCrucibleGameplayAbility
{
	GENERATED_BODY()

public:

	UCrucibleGA_Dodge();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:

	/** Dodge animation montage (optional — leave null for M1 LaunchCharacter fallback) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dodge")
	TObjectPtr<UAnimMontage> DodgeMontage;

	/** Duration of i-frame invulnerability window (seconds) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dodge")
	float IFrameDuration = 0.3f;

	/** Total dodge duration when using LaunchCharacter fallback (seconds) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dodge")
	float DodgeDuration = 0.6f;

	/** Launch speed for the LaunchCharacter fallback (no montage) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dodge")
	float DodgeLaunchSpeed = 1000.f;

private:

	/** Rotate character to face the dodge direction based on movement input */
	void RotateCharacterToDodgeDirection(ACharacter* Character);

	/** Get the dodge direction from current movement input (or backward if none) */
	FVector GetDodgeDirection(const ACharacter* Character) const;

	/** Called when i-frame window expires */
	void EndIFrameWindow();

	/** Called when dodge duration expires (LaunchCharacter fallback only) */
	void EndDodgeTimer();

	UFUNCTION()
	void OnMontageCompleted(UAnimMontage* Montage, bool bInterrupted);

	FTimerHandle IFrameTimerHandle;
	FTimerHandle DodgeDurationTimerHandle;
};
