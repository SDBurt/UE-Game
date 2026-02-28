// Copyright Epic Games, Inc. All Rights Reserved.

#include "CrucibleGA_BasicAttack.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Crucible.h"
#include "GameFramework/Character.h"

UCrucibleGA_BasicAttack::UCrucibleGA_BasicAttack()
{
	AbilityTagName = FName("Ability.BasicAttack");
}

void UCrucibleGA_BasicAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
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

	// Play attack montage if assigned
	if (AttackMontage)
	{
		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(AttackMontage);

			FOnMontageEnded EndDelegate;
			EndDelegate.BindUObject(this, &UCrucibleGA_BasicAttack::OnMontageCompleted);
			AnimInstance->Montage_SetEndDelegate(EndDelegate, AttackMontage);
		}
	}

	// Perform the trace immediately for milestone 1 (later: trigger from anim notify)
	UE_LOG(LogCrucible, Log, TEXT("BasicAttack activated by %s"), *Character->GetName());
	PerformAttackTrace();

	// If no montage, end ability immediately
	if (!AttackMontage)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}

void UCrucibleGA_BasicAttack::PerformAttackTrace()
{
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (!AvatarActor)
	{
		return;
	}

	const FVector Start = AvatarActor->GetActorLocation();
	const FVector End = Start + AvatarActor->GetActorForwardVector() * TraceDistance;

	FCollisionShape Shape;
	Shape.SetSphere(TraceRadius);

	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(AvatarActor);

	bool bHit = AvatarActor->GetWorld()->SweepMultiByChannel(
		HitResults, Start, End, FQuat::Identity, ECC_Pawn, Shape, Params);

	if (bHit && DamageEffectClass)
	{
		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
		if (!SourceASC)
		{
			return;
		}

		for (const FHitResult& Hit : HitResults)
		{
			AActor* HitActor = Hit.GetActor();
			if (!HitActor || HitActor == AvatarActor)
			{
				continue;
			}

			UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor);
			if (TargetASC)
			{
				FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());
				if (SpecHandle.IsValid())
				{
					SourceASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
					UE_LOG(LogCrucible, Log, TEXT("BasicAttack hit %s"), *HitActor->GetName());
				}
			}
		}
	}
}

void UCrucibleGA_BasicAttack::OnMontageCompleted(UAnimMontage* Montage, bool bInterrupted)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, bInterrupted);
}

void UCrucibleGA_BasicAttack::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
