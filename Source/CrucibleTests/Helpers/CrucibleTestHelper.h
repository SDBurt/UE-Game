// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class UWorld;
class ACrucibleTestPawn;
class UGameplayEffect;
class UAbilitySystemComponent;

/**
 * Static helper functions for Crucible automated tests.
 * Handles world creation, pawn spawning, and programmatic GE construction.
 */
struct FCrucibleTestHelper
{
	/** Ensures native gameplay tags are registered (idempotent). */
	static void EnsureTagsRegistered();

	/** Creates a minimal transient UWorld for testing. */
	static UWorld* CreateTestWorld();

	/** Destroys a test world and cleans up. */
	static void DestroyTestWorld(UWorld* World);

	/** Spawns a test pawn in the given world and initializes its ASC. */
	static ACrucibleTestPawn* SpawnTestPawn(UWorld* World);

	/**
	 * Creates a programmatic damage GameplayEffect using CrucibleDamageExecution.
	 * @param DamageAmount  If > 0, sets a SetByCaller magnitude on Damage.Physical.
	 *                      If <= 0, the execution uses its default 10 damage.
	 */
	static UGameplayEffect* CreateDamageGE(float DamageAmount = 0.f);

	/** Applies a GameplayEffect from Source ASC to Target ASC. */
	static void ApplyGEToTarget(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC, UGameplayEffect* Effect, float DamageAmount = 0.f);
};
