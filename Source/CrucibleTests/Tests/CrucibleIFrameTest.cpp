// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "CrucibleTestHelper.h"
#include "CrucibleTestPawn.h"
#include "CrucibleAttributeSet.h"
#include "CrucibleGameplayTags.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"

// --------------------------------------------------------------------------
// DamageBlocked_WhenDodgingTagPresent — Health unchanged with State.Dodging
// --------------------------------------------------------------------------
IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FCrucibleTest_DamageBlocked_WhenDodgingTagPresent,
	"Crucible.IFrame.DamageBlocked_WhenDodgingTagPresent",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCrucibleTest_DamageBlocked_WhenDodgingTagPresent::RunTest(const FString& Parameters)
{
	FCrucibleTestHelper::EnsureTagsRegistered();
	UWorld* World = FCrucibleTestHelper::CreateTestWorld();
	ACrucibleTestPawn* Source = FCrucibleTestHelper::SpawnTestPawn(World);
	ACrucibleTestPawn* Target = FCrucibleTestHelper::SpawnTestPawn(World);
	UGameplayEffect* DamageGE = FCrucibleTestHelper::CreateDamageGE();

	const FCrucibleGameplayTags& Tags = FCrucibleGameplayTags::Get();

	// Add dodge tag before damage
	Target->GetAbilitySystemComponent()->AddLooseGameplayTag(Tags.State_Dodging);

	FCrucibleTestHelper::ApplyGEToTarget(
		Source->GetAbilitySystemComponent(),
		Target->GetAbilitySystemComponent(),
		DamageGE);

	TestEqual(TEXT("Health unchanged during dodge"), Target->GetAttributeSet()->GetHealth(), 100.f);
	TestEqual(TEXT("Stagger unchanged during dodge"), Target->GetAttributeSet()->GetStaggerDamage(), 0.f);

	FCrucibleTestHelper::DestroyTestWorld(World);
	return true;
}

// --------------------------------------------------------------------------
// DamageApplied_WhenDodgingTagAbsent — normal damage without tag
// --------------------------------------------------------------------------
IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FCrucibleTest_DamageApplied_WhenDodgingTagAbsent,
	"Crucible.IFrame.DamageApplied_WhenDodgingTagAbsent",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCrucibleTest_DamageApplied_WhenDodgingTagAbsent::RunTest(const FString& Parameters)
{
	FCrucibleTestHelper::EnsureTagsRegistered();
	UWorld* World = FCrucibleTestHelper::CreateTestWorld();
	ACrucibleTestPawn* Source = FCrucibleTestHelper::SpawnTestPawn(World);
	ACrucibleTestPawn* Target = FCrucibleTestHelper::SpawnTestPawn(World);
	UGameplayEffect* DamageGE = FCrucibleTestHelper::CreateDamageGE();

	// No dodge tag — damage should apply normally
	FCrucibleTestHelper::ApplyGEToTarget(
		Source->GetAbilitySystemComponent(),
		Target->GetAbilitySystemComponent(),
		DamageGE);

	TestEqual(TEXT("Health reduced without dodge"), Target->GetAttributeSet()->GetHealth(), 90.f);

	FCrucibleTestHelper::DestroyTestWorld(World);
	return true;
}

// --------------------------------------------------------------------------
// DamageApplied_AfterTagRemoved — damage works after i-frame ends
// --------------------------------------------------------------------------
IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FCrucibleTest_DamageApplied_AfterTagRemoved,
	"Crucible.IFrame.DamageApplied_AfterTagRemoved",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCrucibleTest_DamageApplied_AfterTagRemoved::RunTest(const FString& Parameters)
{
	FCrucibleTestHelper::EnsureTagsRegistered();
	UWorld* World = FCrucibleTestHelper::CreateTestWorld();
	ACrucibleTestPawn* Source = FCrucibleTestHelper::SpawnTestPawn(World);
	ACrucibleTestPawn* Target = FCrucibleTestHelper::SpawnTestPawn(World);
	UGameplayEffect* DamageGE = FCrucibleTestHelper::CreateDamageGE();

	const FCrucibleGameplayTags& Tags = FCrucibleGameplayTags::Get();

	// Add and then remove dodge tag
	Target->GetAbilitySystemComponent()->AddLooseGameplayTag(Tags.State_Dodging);
	Target->GetAbilitySystemComponent()->RemoveLooseGameplayTag(Tags.State_Dodging);

	FCrucibleTestHelper::ApplyGEToTarget(
		Source->GetAbilitySystemComponent(),
		Target->GetAbilitySystemComponent(),
		DamageGE);

	TestEqual(TEXT("Health reduced after dodge ends"), Target->GetAttributeSet()->GetHealth(), 90.f);

	FCrucibleTestHelper::DestroyTestWorld(World);
	return true;
}
