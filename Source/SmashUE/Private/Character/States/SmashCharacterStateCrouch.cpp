// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/States/SmashCharacterStateCrouch.h"

#include "Character/SmashCharacter.h"
#include "Character/SmashCharacterStateMachine.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


ESmashCharacterStateID USmashCharacterStateCrouch::GetStateID()
{
	return ESmashCharacterStateID::Crouch;
}

void USmashCharacterStateCrouch::OnInputJump(float InputMoveX)
{
	StateMachine->ChangeState(ESmashCharacterStateID::Jump);
}

void USmashCharacterStateCrouch::StateEnter(ESmashCharacterStateID PreviousState)
{
	Super::StateEnter(PreviousState);

	UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement();

	if (MovementComponent != nullptr)
	{
		MovementComponent->MaxWalkSpeed = MoveCrouchSpeedMax;
	}

	Character->PlayAnimMontage(CrouchAnim);

	UCapsuleComponent* CapsuleComponent = Character->GetCapsuleComponent();

	/*
	if (CapsuleComponent != nullptr)
		CapsuleComponent->SetCapsuleHalfHeight(CapsuleComponent->GetUnscaledCapsuleHalfHeight() / 2.f);
	*/
	
	Character->InputJumpEvent.AddDynamic(this, &USmashCharacterStateCrouch::OnInputJump);
}

void USmashCharacterStateCrouch::StateExit(ESmashCharacterStateID NextState)
{
	Super::StateExit(NextState);
	
	UCapsuleComponent* CapsuleComponent = Character->GetCapsuleComponent();

	/*
	if (CapsuleComponent != nullptr)
		CapsuleComponent->SetCapsuleHalfHeight(CapsuleComponent->GetUnscaledCapsuleHalfHeight() * 2.f);
	*/
	
	Character->InputJumpEvent.RemoveDynamic(this, &USmashCharacterStateCrouch::OnInputJump);
}

void USmashCharacterStateCrouch::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	if (Character->GetInputCrouchValue() >= -0.1f)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Idle);
	}
}
