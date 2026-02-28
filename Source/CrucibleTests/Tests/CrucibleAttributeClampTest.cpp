// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "CrucibleTestHelper.h"
#include "CrucibleTestPawn.h"
#include "CrucibleAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"

// --------------------------------------------------------------------------
// Helper: Creates an instant GE that directly modifies a single attribute
// --------------------------------------------------------------------------
static UGameplayEffect* CreateAttributeModGE(const FGameplayAttribute& Attribute, float Value, EGameplayModOp::Type ModOp)
{
	UGameplayEffect* GE = NewObject<UGameplayEffect>(GetTransientPackage(), MakeUniqueObjectName(GetTransientPackage(), UGameplayEffect::StaticClass()));
	GE->DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo Modifier;
	Modifier.Attribute = Attribute;
	Modifier.ModifierOp = ModOp;

	FScalableFloat Magnitude;
	Magnitude.Value = Value;

	FGameplayEffectModifierMagnitude ModMagnitude(Magnitude);
	Modifier.ModifierMagnitude = ModMagnitude;

	GE->Modifiers.Add(Modifier);

	return GE;
}

static void ApplyDirectGE(UAbilitySystemComponent* ASC, UGameplayEffect* GE)
{
	FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
	Context.AddSourceObject(ASC->GetOwnerActor());
	FGameplayEffectSpec Spec(GE, Context, 1.f);
	ASC->ApplyGameplayEffectSpecToSelf(Spec);
}

// --------------------------------------------------------------------------
// HealthClampedToMaxHealth — can't exceed MaxHealth via additive
// --------------------------------------------------------------------------
IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FCrucibleTest_HealthClampedToMaxHealth,
	"Crucible.Clamping.HealthClampedToMaxHealth",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCrucibleTest_HealthClampedToMaxHealth::RunTest(const FString& Parameters)
{
	FCrucibleTestHelper::EnsureTagsRegistered();
	UWorld* World = FCrucibleTestHelper::CreateTestWorld();
	ACrucibleTestPawn* Pawn = FCrucibleTestHelper::SpawnTestPawn(World);

	// Try to add 50 health when already at max (100)
	UGameplayEffect* HealGE = CreateAttributeModGE(
		UCrucibleAttributeSet::GetHealthAttribute(), 50.f, EGameplayModOp::Additive);
	ApplyDirectGE(Pawn->GetAbilitySystemComponent(), HealGE);

	TestEqual(TEXT("Health clamped to MaxHealth"), Pawn->GetAttributeSet()->GetHealth(), 100.f);

	FCrucibleTestHelper::DestroyTestWorld(World);
	return true;
}

// --------------------------------------------------------------------------
// HealthClampedToZero — can't go below 0 via direct subtract
// --------------------------------------------------------------------------
IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FCrucibleTest_HealthClampedToZero,
	"Crucible.Clamping.HealthClampedToZero",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCrucibleTest_HealthClampedToZero::RunTest(const FString& Parameters)
{
	FCrucibleTestHelper::EnsureTagsRegistered();
	UWorld* World = FCrucibleTestHelper::CreateTestWorld();
	ACrucibleTestPawn* Pawn = FCrucibleTestHelper::SpawnTestPawn(World);

	// Subtract 200 from health (starts at 100)
	UGameplayEffect* DamageGE = CreateAttributeModGE(
		UCrucibleAttributeSet::GetHealthAttribute(), -200.f, EGameplayModOp::Additive);
	ApplyDirectGE(Pawn->GetAbilitySystemComponent(), DamageGE);

	TestTrue(TEXT("Health >= 0"), Pawn->GetAttributeSet()->GetHealth() >= 0.f);

	FCrucibleTestHelper::DestroyTestWorld(World);
	return true;
}

// --------------------------------------------------------------------------
// SkillPointsClampedToMax — SP capped at MaxSP
// --------------------------------------------------------------------------
IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FCrucibleTest_SkillPointsClampedToMax,
	"Crucible.Clamping.SkillPointsClampedToMax",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCrucibleTest_SkillPointsClampedToMax::RunTest(const FString& Parameters)
{
	FCrucibleTestHelper::EnsureTagsRegistered();
	UWorld* World = FCrucibleTestHelper::CreateTestWorld();
	ACrucibleTestPawn* Pawn = FCrucibleTestHelper::SpawnTestPawn(World);

	// Try to add 50 SP when already at max (100)
	UGameplayEffect* AddSP = CreateAttributeModGE(
		UCrucibleAttributeSet::GetSkillPointsAttribute(), 50.f, EGameplayModOp::Additive);
	ApplyDirectGE(Pawn->GetAbilitySystemComponent(), AddSP);

	TestEqual(TEXT("SP clamped to MaxSP"), Pawn->GetAttributeSet()->GetSkillPoints(), 100.f);

	FCrucibleTestHelper::DestroyTestWorld(World);
	return true;
}

// --------------------------------------------------------------------------
// StaggerClampedToMax — StaggerDamage capped at MaxStaggerDamage
// --------------------------------------------------------------------------
IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FCrucibleTest_StaggerClampedToMax,
	"Crucible.Clamping.StaggerClampedToMax",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCrucibleTest_StaggerClampedToMax::RunTest(const FString& Parameters)
{
	FCrucibleTestHelper::EnsureTagsRegistered();
	UWorld* World = FCrucibleTestHelper::CreateTestWorld();
	ACrucibleTestPawn* Pawn = FCrucibleTestHelper::SpawnTestPawn(World);

	// Try to add 100 stagger (max is 50, starts at 0)
	UGameplayEffect* AddStagger = CreateAttributeModGE(
		UCrucibleAttributeSet::GetStaggerDamageAttribute(), 100.f, EGameplayModOp::Additive);
	ApplyDirectGE(Pawn->GetAbilitySystemComponent(), AddStagger);

	TestEqual(TEXT("Stagger clamped to max"), Pawn->GetAttributeSet()->GetStaggerDamage(), 50.f);

	FCrucibleTestHelper::DestroyTestWorld(World);
	return true;
}
