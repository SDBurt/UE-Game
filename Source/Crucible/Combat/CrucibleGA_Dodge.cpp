// Copyright Epic Games, Inc. All Rights Reserved.

#include "CrucibleGA_Dodge.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Crucible.h"

UCrucibleGA_Dodge::UCrucibleGA_Dodge()
{
	AbilityTagName = FName("Ability.Dodge");
}

void UCrucibleGA_Dodge::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (!Character)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// Snap rotation to dodge direction
	RotateCharacterToDodgeDirection(Character);

	// Apply i-frame: add State.Dodging tag
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (ASC)
	{
		const FGameplayTag DodgingTag = FGameplayTag::RequestGameplayTag(FName("State.Dodging"), false);
		if (DodgingTag.IsValid())
		{
			ASC->AddLooseGameplayTag(DodgingTag);
			UE_LOG(LogCrucible, Log, TEXT("Dodge: i-frame ON for %s"), *Character->GetName());
		}
	}

	// Set timer to end i-frame window
	Character->GetWorldTimerManager().SetTimer(
		IFrameTimerHandle, this, &UCrucibleGA_Dodge::EndIFrameWindow, IFrameDuration, false);

	// Play montage if assigned, otherwise use LaunchCharacter fallback
	if (DodgeMontage)
	{
		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(DodgeMontage);

			FOnMontageEnded EndDelegate;
			EndDelegate.BindUObject(this, &UCrucibleGA_Dodge::OnMontageCompleted);
			AnimInstance->Montage_SetEndDelegate(EndDelegate, DodgeMontage);
		}
	}
	else
	{
		// M1 fallback: launch character in dodge direction
		const FVector DodgeDirection = GetDodgeDirection(Character);
		const FVector LaunchVelocity = DodgeDirection * DodgeLaunchSpeed;
		Character->LaunchCharacter(LaunchVelocity, true, true);

		UE_LOG(LogCrucible, Log, TEXT("Dodge: launched %s in direction (%.1f, %.1f, %.1f)"),
			*Character->GetName(), DodgeDirection.X, DodgeDirection.Y, DodgeDirection.Z);

		// Set timer to end ability after dodge duration
		Character->GetWorldTimerManager().SetTimer(
			DodgeDurationTimerHandle, this, &UCrucibleGA_Dodge::EndDodgeTimer, DodgeDuration, false);
	}
}

FVector UCrucibleGA_Dodge::GetDodgeDirection(const ACharacter* Character) const
{
	if (!Character)
	{
		return FVector::BackwardVector;
	}

	// Get the last movement input vector from CharacterMovement
	const FVector LastInput = Character->GetCharacterMovement()->GetLastInputVector();

	if (LastInput.SizeSquared() > KINDA_SMALL_NUMBER)
	{
		// Dodge in the direction of current movement input
		return LastInput.GetSafeNormal();
	}

	// No input — dodge backward (opposite of character facing)
	return -Character->GetActorForwardVector();
}

void UCrucibleGA_Dodge::RotateCharacterToDodgeDirection(ACharacter* Character)
{
	if (!Character)
	{
		return;
	}

	const FVector DodgeDir = GetDodgeDirection(Character);

	// Only snap rotation if there's a valid direction
	if (DodgeDir.SizeSquared() > KINDA_SMALL_NUMBER)
	{
		const FRotator NewRotation = DodgeDir.Rotation();
		Character->SetActorRotation(FRotator(0.f, NewRotation.Yaw, 0.f));
	}
}

void UCrucibleGA_Dodge::EndIFrameWindow()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (ASC)
	{
		const FGameplayTag DodgingTag = FGameplayTag::RequestGameplayTag(FName("State.Dodging"), false);
		if (DodgingTag.IsValid())
		{
			ASC->RemoveLooseGameplayTag(DodgingTag);
			UE_LOG(LogCrucible, Log, TEXT("Dodge: i-frame OFF"));
		}
	}
}

void UCrucibleGA_Dodge::EndDodgeTimer()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UCrucibleGA_Dodge::OnMontageCompleted(UAnimMontage* Montage, bool bInterrupted)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, bInterrupted);
}

void UCrucibleGA_Dodge::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	// Safety: ensure i-frame tag is removed even if timers were interrupted
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (ASC)
	{
		const FGameplayTag DodgingTag = FGameplayTag::RequestGameplayTag(FName("State.Dodging"), false);
		if (DodgingTag.IsValid() && ASC->HasMatchingGameplayTag(DodgingTag))
		{
			ASC->RemoveLooseGameplayTag(DodgingTag);
			UE_LOG(LogCrucible, Log, TEXT("Dodge: safety-removed i-frame tag in EndAbility"));
		}
	}

	// Clear timers
	if (ActorInfo && ActorInfo->AvatarActor.IsValid())
	{
		AActor* Avatar = ActorInfo->AvatarActor.Get();
		Avatar->GetWorldTimerManager().ClearTimer(IFrameTimerHandle);
		Avatar->GetWorldTimerManager().ClearTimer(DodgeDurationTimerHandle);
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
