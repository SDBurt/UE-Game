// Copyright Epic Games, Inc. All Rights Reserved.

#include "CrucibleAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemComponent.h"
#include "CrucibleGameplayTags.h"
#include "Crucible.h"

UCrucibleAttributeSet::UCrucibleAttributeSet()
{
	InitHealth(100.f);
	InitMaxHealth(100.f);
	InitSkillPoints(100.f);
	InitMaxSkillPoints(100.f);
	InitStaggerDamage(0.f);
	InitMaxStaggerDamage(50.f);
	InitIncomingDamage(0.f);
}

void UCrucibleAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	else if (Attribute == GetSkillPointsAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxSkillPoints());
	}
	else if (Attribute == GetStaggerDamageAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxStaggerDamage());
	}
}

void UCrucibleAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		const float DamageDone = GetIncomingDamage();
		SetIncomingDamage(0.f);

		if (DamageDone > 0.f)
		{
			// Subtract from Health
			const float NewHealth = GetHealth() - DamageDone;
			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

			// Add to StaggerDamage
			const float NewStagger = GetStaggerDamage() + DamageDone;
			SetStaggerDamage(FMath::Clamp(NewStagger, 0.f, GetMaxStaggerDamage()));

			UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();
			if (!ASC)
			{
				return;
			}

			const FCrucibleGameplayTags& Tags = FCrucibleGameplayTags::Get();

			// Check death
			if (GetHealth() <= 0.f)
			{
				ASC->AddLooseGameplayTag(Tags.State_Dead);
				UE_LOG(LogCrucible, Log, TEXT("%s is dead."), *ASC->GetOwnerActor()->GetName());
			}

			// Check stagger
			if (GetStaggerDamage() >= GetMaxStaggerDamage())
			{
				ASC->AddLooseGameplayTag(Tags.Status_Staggered);
				UE_LOG(LogCrucible, Log, TEXT("%s is staggered."), *ASC->GetOwnerActor()->GetName());
			}
		}
	}
}
