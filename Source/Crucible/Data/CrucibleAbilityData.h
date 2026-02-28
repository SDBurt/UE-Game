// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "CrucibleAbilityData.generated.h"

class UGameplayAbility;
class UAnimMontage;
class UTexture2D;

/**
 * Data asset describing a single ability for UI and configuration.
 * Create instances in the editor to define each skill's properties.
 */
UCLASS(BlueprintType)
class UCrucibleAbilityData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	/** Display name shown in UI */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	FText AbilityName;

	/** Icon for the skill slot */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	TObjectPtr<UTexture2D> AbilityIcon;

	/** The gameplay ability class this data drives */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	TSubclassOf<UGameplayAbility> AbilityClass;

	/** Element tag (Element.Heat, Element.Electric, etc.) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	FGameplayTag ElementTag;

	/** Base damage value */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	float BaseDamage = 10.f;

	/** Cooldown in seconds */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	float Cooldown = 0.f;

	/** Skill Point cost */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	float SPCost = 0.f;

	/** Attack animation montage */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	TObjectPtr<UAnimMontage> Montage;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("CrucibleAbility", GetFName());
	}
};
