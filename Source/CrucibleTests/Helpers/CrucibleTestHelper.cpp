// Copyright Epic Games, Inc. All Rights Reserved.

#include "CrucibleTestHelper.h"
#include "CrucibleTestPawn.h"
#include "CrucibleGameplayTags.h"
#include "CrucibleDamageExecution.h"
#include "CrucibleAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Engine/World.h"

static bool bTagsRegistered = false;

void FCrucibleTestHelper::EnsureTagsRegistered()
{
	if (!bTagsRegistered)
	{
		FCrucibleGameplayTags::InitializeNativeTags();
		bTagsRegistered = true;
	}
}

UWorld* FCrucibleTestHelper::CreateTestWorld()
{
	UWorld* World = UWorld::CreateWorld(EWorldType::Game, false);
	FWorldContext& WorldContext = GEngine->CreateNewWorldContext(EWorldType::Game);
	WorldContext.SetCurrentWorld(World);
	World->InitializeActorsForPlay(FURL());
	World->BeginPlay();
	return World;
}

void FCrucibleTestHelper::DestroyTestWorld(UWorld* World)
{
	if (!World)
	{
		return;
	}

	GEngine->DestroyWorldContext(World);
	World->DestroyWorld(false);
}

ACrucibleTestPawn* FCrucibleTestHelper::SpawnTestPawn(UWorld* World)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ACrucibleTestPawn* Pawn = World->SpawnActor<ACrucibleTestPawn>(SpawnParams);

	// Initialize ASC — owner actor is the pawn itself
	UAbilitySystemComponent* ASC = Pawn->GetAbilitySystemComponent();
	ASC->InitAbilityActorInfo(Pawn, Pawn);

	return Pawn;
}

UGameplayEffect* FCrucibleTestHelper::CreateDamageGE(float DamageAmount)
{
	UGameplayEffect* GE = NewObject<UGameplayEffect>(GetTransientPackage(), FName("GE_TestDamage"));
	GE->DurationPolicy = EGameplayEffectDurationType::Instant;

	// Add execution calculation using CrucibleDamageExecution
	FGameplayEffectExecutionDefinition Execution;
	Execution.CalculationClass = UCrucibleDamageExecution::StaticClass();
	GE->Executions.Add(Execution);

	return GE;
}

void FCrucibleTestHelper::ApplyGEToTarget(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC, UGameplayEffect* Effect, float DamageAmount)
{
	FGameplayEffectContextHandle Context = SourceASC->MakeEffectContext();
	Context.AddSourceObject(SourceASC->GetOwnerActor());

	// Construct spec directly from the GE instance (MakeOutgoingSpec requires TSubclassOf, not an instance)
	FGameplayEffectSpec Spec(Effect, Context, 1.f);

	if (DamageAmount > 0.f)
	{
		const FGameplayTag DamageTag = FGameplayTag::RequestGameplayTag(FName("Damage.Physical"), false);
		Spec.SetSetByCallerMagnitude(DamageTag, DamageAmount);
	}

	TargetASC->ApplyGameplayEffectSpecToSelf(Spec);
}
