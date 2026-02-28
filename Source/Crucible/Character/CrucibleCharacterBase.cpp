// Copyright Epic Games, Inc. All Rights Reserved.

#include "CrucibleCharacterBase.h"
#include "CrucibleAbilitySystemComponent.h"
#include "CrucibleAttributeSet.h"
#include "Crucible.h"

ACrucibleCharacterBase::ACrucibleCharacterBase()
{
	AbilitySystemComponent = CreateDefaultSubobject<UCrucibleAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UCrucibleAttributeSet>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* ACrucibleCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ACrucibleCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		ApplyDefaultAttributes();
		GiveDefaultAbilities();
	}
}

void ACrucibleCharacterBase::GiveDefaultAbilities()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	for (const TSubclassOf<UGameplayAbility>& AbilityClass : DefaultAbilities)
	{
		if (AbilityClass)
		{
			FGameplayAbilitySpec Spec(AbilityClass, 1, INDEX_NONE, this);
			AbilitySystemComponent->GiveAbility(Spec);
		}
	}
}

void ACrucibleCharacterBase::ApplyDefaultAttributes()
{
	if (!AbilitySystemComponent || !DefaultAttributeEffect)
	{
		return;
	}

	FGameplayEffectContextHandle Context = AbilitySystemComponent->MakeEffectContext();
	Context.AddSourceObject(this);

	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect, 1, Context);
	if (SpecHandle.IsValid())
	{
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		UE_LOG(LogCrucible, Log, TEXT("Applied default attributes to %s"), *GetName());
	}
}
