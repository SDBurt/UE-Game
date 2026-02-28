// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "CrucibleTestHelper.h"
#include "CrucibleTestPawn.h"
#include "CrucibleAttributeSet.h"
#include "CrucibleGameplayTags.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"

// --------------------------------------------------------------------------
// SingleHit_ReducesHealth — 100 -> 90 after one 10-damage hit
// --------------------------------------------------------------------------
IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FCrucibleTest_SingleHit_ReducesHealth,
	"Crucible.Damage.SingleHit_ReducesHealth",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCrucibleTest_SingleHit_ReducesHealth::RunTest(const FString& Parameters)
{
	FCrucibleTestHelper::EnsureTagsRegistered();
	UWorld* World = FCrucibleTestHelper::CreateTestWorld();
	ACrucibleTestPawn* Source = FCrucibleTestHelper::SpawnTestPawn(World);
	ACrucibleTestPawn* Target = FCrucibleTestHelper::SpawnTestPawn(World);
	UGameplayEffect* DamageGE = FCrucibleTestHelper::CreateDamageGE();

	FCrucibleTestHelper::ApplyGEToTarget(
		Source->GetAbilitySystemComponent(),
		Target->GetAbilitySystemComponent(),
		DamageGE);

	TestEqual(TEXT("Health after 1 hit"), Target->GetAttributeSet()->GetHealth(), 90.f);

	FCrucibleTestHelper::DestroyTestWorld(World);
	return true;
}

// --------------------------------------------------------------------------
// SingleHit_AccumulatesStagger — 0 -> 10 stagger after one hit
// --------------------------------------------------------------------------
IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FCrucibleTest_SingleHit_AccumulatesStagger,
	"Crucible.Damage.SingleHit_AccumulatesStagger",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCrucibleTest_SingleHit_AccumulatesStagger::RunTest(const FString& Parameters)
{
	FCrucibleTestHelper::EnsureTagsRegistered();
	UWorld* World = FCrucibleTestHelper::CreateTestWorld();
	ACrucibleTestPawn* Source = FCrucibleTestHelper::SpawnTestPawn(World);
	ACrucibleTestPawn* Target = FCrucibleTestHelper::SpawnTestPawn(World);
	UGameplayEffect* DamageGE = FCrucibleTestHelper::CreateDamageGE();

	FCrucibleTestHelper::ApplyGEToTarget(
		Source->GetAbilitySystemComponent(),
		Target->GetAbilitySystemComponent(),
		DamageGE);

	TestEqual(TEXT("Stagger after 1 hit"), Target->GetAttributeSet()->GetStaggerDamage(), 10.f);

	FCrucibleTestHelper::DestroyTestWorld(World);
	return true;
}

// --------------------------------------------------------------------------
// HealthClampsToZero — 150 damage should leave Health at 0, not negative
// --------------------------------------------------------------------------
IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FCrucibleTest_HealthClampsToZero,
	"Crucible.Damage.HealthClampsToZero",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCrucibleTest_HealthClampsToZero::RunTest(const FString& Parameters)
{
	FCrucibleTestHelper::EnsureTagsRegistered();
	UWorld* World = FCrucibleTestHelper::CreateTestWorld();
	ACrucibleTestPawn* Source = FCrucibleTestHelper::SpawnTestPawn(World);
	ACrucibleTestPawn* Target = FCrucibleTestHelper::SpawnTestPawn(World);
	UGameplayEffect* DamageGE = FCrucibleTestHelper::CreateDamageGE();

	// Apply 150 damage via SetByCaller
	FCrucibleTestHelper::ApplyGEToTarget(
		Source->GetAbilitySystemComponent(),
		Target->GetAbilitySystemComponent(),
		DamageGE, 150.f);

	TestEqual(TEXT("Health clamped to 0"), Target->GetAttributeSet()->GetHealth(), 0.f);

	FCrucibleTestHelper::DestroyTestWorld(World);
	return true;
}

// --------------------------------------------------------------------------
// StaggerClampsToMax — stagger capped at MaxStaggerDamage (50)
// --------------------------------------------------------------------------
IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FCrucibleTest_StaggerClampsToMax,
	"Crucible.Damage.StaggerClampsToMax",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCrucibleTest_StaggerClampsToMax::RunTest(const FString& Parameters)
{
	FCrucibleTestHelper::EnsureTagsRegistered();
	UWorld* World = FCrucibleTestHelper::CreateTestWorld();
	ACrucibleTestPawn* Source = FCrucibleTestHelper::SpawnTestPawn(World);
	ACrucibleTestPawn* Target = FCrucibleTestHelper::SpawnTestPawn(World);
	UGameplayEffect* DamageGE = FCrucibleTestHelper::CreateDamageGE();

	// Apply 150 damage — stagger should cap at 50
	FCrucibleTestHelper::ApplyGEToTarget(
		Source->GetAbilitySystemComponent(),
		Target->GetAbilitySystemComponent(),
		DamageGE, 150.f);

	TestEqual(TEXT("Stagger capped at max"), Target->GetAttributeSet()->GetStaggerDamage(), 50.f);

	FCrucibleTestHelper::DestroyTestWorld(World);
	return true;
}

// --------------------------------------------------------------------------
// DeathTag_AppliedAtZeroHealth — State.Dead tag when Health = 0
// --------------------------------------------------------------------------
IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FCrucibleTest_DeathTag_AppliedAtZeroHealth,
	"Crucible.Damage.DeathTag_AppliedAtZeroHealth",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCrucibleTest_DeathTag_AppliedAtZeroHealth::RunTest(const FString& Parameters)
{
	FCrucibleTestHelper::EnsureTagsRegistered();
	UWorld* World = FCrucibleTestHelper::CreateTestWorld();
	ACrucibleTestPawn* Source = FCrucibleTestHelper::SpawnTestPawn(World);
	ACrucibleTestPawn* Target = FCrucibleTestHelper::SpawnTestPawn(World);
	UGameplayEffect* DamageGE = FCrucibleTestHelper::CreateDamageGE();

	FCrucibleTestHelper::ApplyGEToTarget(
		Source->GetAbilitySystemComponent(),
		Target->GetAbilitySystemComponent(),
		DamageGE, 100.f);

	const FCrucibleGameplayTags& Tags = FCrucibleGameplayTags::Get();
	TestTrue(TEXT("State.Dead tag applied"),
		Target->GetAbilitySystemComponent()->HasMatchingGameplayTag(Tags.State_Dead));

	FCrucibleTestHelper::DestroyTestWorld(World);
	return true;
}

// --------------------------------------------------------------------------
// StaggerTag_AppliedAtMaxStagger — Status.Staggered tag when stagger full
// --------------------------------------------------------------------------
IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FCrucibleTest_StaggerTag_AppliedAtMaxStagger,
	"Crucible.Damage.StaggerTag_AppliedAtMaxStagger",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCrucibleTest_StaggerTag_AppliedAtMaxStagger::RunTest(const FString& Parameters)
{
	FCrucibleTestHelper::EnsureTagsRegistered();
	UWorld* World = FCrucibleTestHelper::CreateTestWorld();
	ACrucibleTestPawn* Source = FCrucibleTestHelper::SpawnTestPawn(World);
	ACrucibleTestPawn* Target = FCrucibleTestHelper::SpawnTestPawn(World);
	UGameplayEffect* DamageGE = FCrucibleTestHelper::CreateDamageGE();

	// 50 damage = fills stagger bar (MaxStagger=50)
	FCrucibleTestHelper::ApplyGEToTarget(
		Source->GetAbilitySystemComponent(),
		Target->GetAbilitySystemComponent(),
		DamageGE, 50.f);

	const FCrucibleGameplayTags& Tags = FCrucibleGameplayTags::Get();
	TestTrue(TEXT("Status.Staggered tag applied"),
		Target->GetAbilitySystemComponent()->HasMatchingGameplayTag(Tags.Status_Staggered));

	FCrucibleTestHelper::DestroyTestWorld(World);
	return true;
}

// --------------------------------------------------------------------------
// MultipleHits_HealthDecrements — 10 hits x 10 dmg = 0
// --------------------------------------------------------------------------
IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FCrucibleTest_MultipleHits_HealthDecrements,
	"Crucible.Damage.MultipleHits_HealthDecrements",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCrucibleTest_MultipleHits_HealthDecrements::RunTest(const FString& Parameters)
{
	FCrucibleTestHelper::EnsureTagsRegistered();
	UWorld* World = FCrucibleTestHelper::CreateTestWorld();
	ACrucibleTestPawn* Source = FCrucibleTestHelper::SpawnTestPawn(World);
	ACrucibleTestPawn* Target = FCrucibleTestHelper::SpawnTestPawn(World);
	UGameplayEffect* DamageGE = FCrucibleTestHelper::CreateDamageGE();

	for (int32 i = 0; i < 10; ++i)
	{
		FCrucibleTestHelper::ApplyGEToTarget(
			Source->GetAbilitySystemComponent(),
			Target->GetAbilitySystemComponent(),
			DamageGE);
	}

	TestEqual(TEXT("Health after 10 hits"), Target->GetAttributeSet()->GetHealth(), 0.f);

	FCrucibleTestHelper::DestroyTestWorld(World);
	return true;
}

// --------------------------------------------------------------------------
// IncomingDamage_ResetsToZero — meta attribute resets after processing
// --------------------------------------------------------------------------
IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FCrucibleTest_IncomingDamage_ResetsToZero,
	"Crucible.Damage.IncomingDamage_ResetsToZero",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCrucibleTest_IncomingDamage_ResetsToZero::RunTest(const FString& Parameters)
{
	FCrucibleTestHelper::EnsureTagsRegistered();
	UWorld* World = FCrucibleTestHelper::CreateTestWorld();
	ACrucibleTestPawn* Source = FCrucibleTestHelper::SpawnTestPawn(World);
	ACrucibleTestPawn* Target = FCrucibleTestHelper::SpawnTestPawn(World);
	UGameplayEffect* DamageGE = FCrucibleTestHelper::CreateDamageGE();

	FCrucibleTestHelper::ApplyGEToTarget(
		Source->GetAbilitySystemComponent(),
		Target->GetAbilitySystemComponent(),
		DamageGE);

	TestEqual(TEXT("IncomingDamage reset to 0"), Target->GetAttributeSet()->GetIncomingDamage(), 0.f);

	FCrucibleTestHelper::DestroyTestWorld(World);
	return true;
}
