// Copyright Epic Games, Inc. All Rights Reserved.

#include "CrucibleTargetDummy.h"
#include "CrucibleAbilitySystemComponent.h"
#include "CrucibleAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Crucible.h"

ACrucibleTargetDummy::ACrucibleTargetDummy()
{
	// Dummies don't move
	GetCharacterMovement()->GravityScale = 1.f;
	GetCharacterMovement()->MaxWalkSpeed = 0.f;
}

void ACrucibleTargetDummy::BeginPlay()
{
	Super::BeginPlay();

	// Listen for attribute changes on Health to fire the BP event
	if (AbilitySystemComponent && AttributeSet)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			UCrucibleAttributeSet::GetHealthAttribute()
		).AddLambda([this](const FOnAttributeChangeData& Data)
		{
			const float DamageAmount = Data.OldValue - Data.NewValue;
			if (DamageAmount > 0.f)
			{
				UE_LOG(LogCrucible, Log, TEXT("TargetDummy took %.1f damage. Health: %.1f -> %.1f"),
					DamageAmount, Data.OldValue, Data.NewValue);

				// Fire Blueprint event
				OnDamageTaken(DamageAmount, nullptr);
			}
		});
	}
}
