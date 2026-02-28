// Copyright Epic Games, Inc. All Rights Reserved.

#include "CruciblePlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "CrucibleAbilitySystemComponent.h"
#include "CrucibleGameplayTags.h"
#include "Crucible.h"

ACruciblePlayerCharacter::ACruciblePlayerCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void ACruciblePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EnhancedInput)
	{
		UE_LOG(LogCrucible, Error, TEXT("Failed to find Enhanced Input Component on %s"), *GetNameSafe(this));
		return;
	}

	// Movement
	if (MoveAction)
	{
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACruciblePlayerCharacter::HandleMove);
	}
	if (LookAction)
	{
		EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACruciblePlayerCharacter::HandleLook);
	}

	// Jump
	if (JumpAction)
	{
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	}

	// Abilities
	if (BasicAttackAction)
	{
		EnhancedInput->BindAction(BasicAttackAction, ETriggerEvent::Started, this, &ACruciblePlayerCharacter::HandleBasicAttack);
	}
	if (SprintDodgeAction)
	{
		EnhancedInput->BindAction(SprintDodgeAction, ETriggerEvent::Started, this, &ACruciblePlayerCharacter::HandleSprintDodgeStarted);
		EnhancedInput->BindAction(SprintDodgeAction, ETriggerEvent::Triggered, this, &ACruciblePlayerCharacter::HandleSprintDodgeOngoing);
		EnhancedInput->BindAction(SprintDodgeAction, ETriggerEvent::Completed, this, &ACruciblePlayerCharacter::HandleSprintDodgeCompleted);
	}
	if (Skill1Action)
	{
		EnhancedInput->BindAction(Skill1Action, ETriggerEvent::Started, this, &ACruciblePlayerCharacter::HandleSkill1);
	}
	if (Skill2Action)
	{
		EnhancedInput->BindAction(Skill2Action, ETriggerEvent::Started, this, &ACruciblePlayerCharacter::HandleSkill2);
	}
	if (Skill3Action)
	{
		EnhancedInput->BindAction(Skill3Action, ETriggerEvent::Started, this, &ACruciblePlayerCharacter::HandleSkill3);
	}
	if (Skill4Action)
	{
		EnhancedInput->BindAction(Skill4Action, ETriggerEvent::Started, this, &ACruciblePlayerCharacter::HandleSkill4);
	}
}

void ACruciblePlayerCharacter::HandleMove(const FInputActionValue& Value)
{
	const FVector2D MoveVector = Value.Get<FVector2D>();

	if (GetController())
	{
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDir, MoveVector.Y);
		AddMovementInput(RightDir, MoveVector.X);
	}
}

void ACruciblePlayerCharacter::HandleLook(const FInputActionValue& Value)
{
	const FVector2D LookVector = Value.Get<FVector2D>();

	if (GetController())
	{
		AddControllerYawInput(LookVector.X);
		AddControllerPitchInput(LookVector.Y);
	}
}

void ACruciblePlayerCharacter::TryActivateAbilityByTag(FGameplayTag AbilityTag)
{
	if (!AbilitySystemComponent || !AbilityTag.IsValid())
	{
		return;
	}

	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(AbilityTag);
	AbilitySystemComponent->TryActivateAbilitiesByTag(TagContainer);
}

void ACruciblePlayerCharacter::HandleBasicAttack()
{
	StopSprint();
	TryActivateAbilityByTag(FGameplayTag::RequestGameplayTag(FName("Ability.BasicAttack")));
}

void ACruciblePlayerCharacter::HandleDodge()
{
	TryActivateAbilityByTag(FGameplayTag::RequestGameplayTag(FName("Ability.Dodge")));
}

void ACruciblePlayerCharacter::HandleSkill1()
{
	StopSprint();
	TryActivateAbilityByTag(FGameplayTag::RequestGameplayTag(FName("Ability.Skill1")));
}

void ACruciblePlayerCharacter::HandleSkill2()
{
	StopSprint();
	TryActivateAbilityByTag(FGameplayTag::RequestGameplayTag(FName("Ability.Skill2")));
}

void ACruciblePlayerCharacter::HandleSkill3()
{
	StopSprint();
	TryActivateAbilityByTag(FGameplayTag::RequestGameplayTag(FName("Ability.Skill3")));
}

void ACruciblePlayerCharacter::HandleSkill4()
{
	StopSprint();
	TryActivateAbilityByTag(FGameplayTag::RequestGameplayTag(FName("Ability.Skill4")));
}

// --- Sprint / Dodge shared key ---

void ACruciblePlayerCharacter::HandleSprintDodgeStarted()
{
	bSprintKeyHeld = true;
	SprintKeyPressedTime = GetWorld()->GetTimeSeconds();
	UE_LOG(LogCrucible, Log, TEXT("SprintDodge: Key DOWN at %.3f"), SprintKeyPressedTime);
}

void ACruciblePlayerCharacter::HandleSprintDodgeOngoing()
{
	const double Elapsed = GetWorld()->GetTimeSeconds() - SprintKeyPressedTime;
	UE_LOG(LogCrucible, Verbose, TEXT("SprintDodge: Ongoing elapsed=%.3f threshold=%.3f"), Elapsed, SprintHoldThreshold);

	// Block if dead or stunned
	if (AbilitySystemComponent)
	{
		const FCrucibleGameplayTags& CrucibleTags = FCrucibleGameplayTags::Get();
		if (AbilitySystemComponent->HasMatchingGameplayTag(CrucibleTags.State_Dead) ||
			AbilitySystemComponent->HasMatchingGameplayTag(CrucibleTags.State_Stunned))
		{
			StopSprint();
			return;
		}
	}

	if (Elapsed < SprintHoldThreshold)
	{
		return; // Still in tap window — wait
	}

	// Past threshold: manage sprint state
	const FVector Velocity = GetCharacterMovement()->Velocity;
	const bool bHasMovementInput = Velocity.SizeSquared2D() > KINDA_SMALL_NUMBER;

	UE_LOG(LogCrucible, Log, TEXT("SprintDodge: Past threshold. HasMovement=%d IsSprinting=%d Velocity=(%.1f, %.1f)"),
		bHasMovementInput, bIsSprinting, Velocity.X, Velocity.Y);

	if (bHasMovementInput && !bIsSprinting)
	{
		StartSprint();
	}
	else if (!bHasMovementInput && bIsSprinting)
	{
		StopSprint();
	}
}

void ACruciblePlayerCharacter::HandleSprintDodgeCompleted()
{
	bSprintKeyHeld = false;
	const double Elapsed = GetWorld()->GetTimeSeconds() - SprintKeyPressedTime;
	UE_LOG(LogCrucible, Log, TEXT("SprintDodge: Key UP elapsed=%.3f wasSprinting=%d"), Elapsed, bIsSprinting);

	if (bIsSprinting)
	{
		StopSprint();
		return;
	}

	// Tap: elapsed < threshold → dodge
	if (Elapsed < SprintHoldThreshold)
	{
		HandleDodge();
	}
}

void ACruciblePlayerCharacter::StartSprint()
{
	bIsSprinting = true;
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	UE_LOG(LogCrucible, Log, TEXT("SprintDodge: Sprint START (speed=%.0f)"), SprintSpeed);

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->AddLooseGameplayTag(FCrucibleGameplayTags::Get().State_Sprinting);
	}
}

void ACruciblePlayerCharacter::StopSprint()
{
	if (!bIsSprinting)
	{
		return;
	}

	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
	UE_LOG(LogCrucible, Log, TEXT("SprintDodge: Sprint STOP (speed=%.0f)"), DefaultWalkSpeed);

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->RemoveLooseGameplayTag(FCrucibleGameplayTags::Get().State_Sprinting);
	}
}
