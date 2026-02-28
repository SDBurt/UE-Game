// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "CrucibleCharacterBase.h"
#include "CruciblePlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

/**
 * Player-controlled character for Crucible.
 * Adds camera boom, Enhanced Input bindings, and ability activation by tag.
 */
UCLASS()
class ACruciblePlayerCharacter : public ACrucibleCharacterBase
{
	GENERATED_BODY()

public:

	ACruciblePlayerCharacter();

	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:

	// --- Components ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	// --- Input Actions ---
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> BasicAttackAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SprintDodgeAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Skill1Action;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Skill2Action;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Skill3Action;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Skill4Action;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	// --- Sprint Config ---
	UPROPERTY(EditDefaultsOnly, Category = "Movement|Sprint", meta = (AllowPrivateAccess = "true"))
	float DefaultWalkSpeed = 500.f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement|Sprint", meta = (AllowPrivateAccess = "true"))
	float SprintSpeed = 900.f;

	/** Time threshold (seconds) to distinguish tap (dodge) from hold (sprint) */
	UPROPERTY(EditDefaultsOnly, Category = "Movement|Sprint", meta = (AllowPrivateAccess = "true"))
	float SprintHoldThreshold = 0.15f;

	// --- Sprint State ---
	bool bSprintKeyHeld = false;
	bool bIsSprinting = false;
	double SprintKeyPressedTime = 0.0;

	// --- Input Handlers ---
	void HandleMove(const FInputActionValue& Value);
	void HandleLook(const FInputActionValue& Value);

	/** Try to activate an ability by its gameplay tag */
	void TryActivateAbilityByTag(FGameplayTag AbilityTag);

	void HandleBasicAttack();
	void HandleDodge();
	void HandleSkill1();
	void HandleSkill2();
	void HandleSkill3();
	void HandleSkill4();

	// --- Sprint/Dodge Shared Key ---
	void HandleSprintDodgeStarted();
	void HandleSprintDodgeOngoing();
	void HandleSprintDodgeCompleted();
	void StartSprint();
	void StopSprint();
};
