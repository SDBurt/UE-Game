// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CruciblePlayerController.generated.h"

class UInputMappingContext;

/**
 * Base PlayerController for Crucible.
 * Adds the default InputMappingContext on BeginPlay.
 */
UCLASS()
class ACruciblePlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	/** Default input mapping context applied on BeginPlay */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	virtual void BeginPlay() override;
};
