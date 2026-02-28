// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "CrucibleTestHelper.h"
#include "CrucibleGameplayTags.h"
#include "GameplayTagContainer.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FCrucibleTest_AllAbilityTagsRegistered,
	"Crucible.Tags.AllAbilityTagsRegistered",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCrucibleTest_AllAbilityTagsRegistered::RunTest(const FString& Parameters)
{
	FCrucibleTestHelper::EnsureTagsRegistered();
	const FCrucibleGameplayTags& Tags = FCrucibleGameplayTags::Get();

	TestTrue(TEXT("Ability.BasicAttack is valid"), Tags.Ability_BasicAttack.IsValid());
	TestTrue(TEXT("Ability.Dodge is valid"), Tags.Ability_Dodge.IsValid());
	TestTrue(TEXT("Ability.Skill1 is valid"), Tags.Ability_Skill1.IsValid());
	TestTrue(TEXT("Ability.Skill2 is valid"), Tags.Ability_Skill2.IsValid());
	TestTrue(TEXT("Ability.Skill3 is valid"), Tags.Ability_Skill3.IsValid());
	TestTrue(TEXT("Ability.Skill4 is valid"), Tags.Ability_Skill4.IsValid());
	TestTrue(TEXT("Ability.Ultimate is valid"), Tags.Ability_Ultimate.IsValid());
	TestTrue(TEXT("Ability.ChainAttack is valid"), Tags.Ability_ChainAttack.IsValid());

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FCrucibleTest_AllStateTagsRegistered,
	"Crucible.Tags.AllStateTagsRegistered",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCrucibleTest_AllStateTagsRegistered::RunTest(const FString& Parameters)
{
	FCrucibleTestHelper::EnsureTagsRegistered();
	const FCrucibleGameplayTags& Tags = FCrucibleGameplayTags::Get();

	TestTrue(TEXT("State.Dead is valid"), Tags.State_Dead.IsValid());
	TestTrue(TEXT("State.Stunned is valid"), Tags.State_Stunned.IsValid());
	TestTrue(TEXT("State.Attacking is valid"), Tags.State_Attacking.IsValid());
	TestTrue(TEXT("State.Dodging is valid"), Tags.State_Dodging.IsValid());

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FCrucibleTest_AllElementTagsRegistered,
	"Crucible.Tags.AllElementTagsRegistered",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCrucibleTest_AllElementTagsRegistered::RunTest(const FString& Parameters)
{
	FCrucibleTestHelper::EnsureTagsRegistered();
	const FCrucibleGameplayTags& Tags = FCrucibleGameplayTags::Get();

	TestTrue(TEXT("Element.Heat is valid"), Tags.Element_Heat.IsValid());
	TestTrue(TEXT("Element.Electric is valid"), Tags.Element_Electric.IsValid());
	TestTrue(TEXT("Element.Cryo is valid"), Tags.Element_Cryo.IsValid());
	TestTrue(TEXT("Element.Nature is valid"), Tags.Element_Nature.IsValid());

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FCrucibleTest_AllStatusTagsRegistered,
	"Crucible.Tags.AllStatusTagsRegistered",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCrucibleTest_AllStatusTagsRegistered::RunTest(const FString& Parameters)
{
	FCrucibleTestHelper::EnsureTagsRegistered();
	const FCrucibleGameplayTags& Tags = FCrucibleGameplayTags::Get();

	TestTrue(TEXT("Status.Staggered is valid"), Tags.Status_Staggered.IsValid());
	TestTrue(TEXT("Status.Burning is valid"), Tags.Status_Burning.IsValid());
	TestTrue(TEXT("Status.Frozen is valid"), Tags.Status_Frozen.IsValid());
	TestTrue(TEXT("Status.Shocked is valid"), Tags.Status_Shocked.IsValid());
	TestTrue(TEXT("Status.Poisoned is valid"), Tags.Status_Poisoned.IsValid());

	return true;
}
