// Copyright Epic Games, Inc. All Rights Reserved.

#include "CrucibleTestPawn.h"
#include "CrucibleAbilitySystemComponent.h"
#include "CrucibleAttributeSet.h"

ACrucibleTestPawn::ACrucibleTestPawn()
{
	AbilitySystemComponent = CreateDefaultSubobject<UCrucibleAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UCrucibleAttributeSet>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* ACrucibleTestPawn::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
