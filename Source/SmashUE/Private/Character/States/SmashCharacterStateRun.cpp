// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/States/SmashCharacterStateRun.h"
#include "Character/SmashCharacter.h"
#include "Character/SmashCharacterStateID.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/SmashCharacterSettings.h"
#include "Character/SmashCharacterStateMachine.h"


ESmashCharacterStateID USmashCharacterStateRun::GetStateID()
{
	return ESmashCharacterStateID::Run;
}

void USmashCharacterStateRun::OnInputJump(float InputMoveX)
{
	StateMachine->ChangeState(ESmashCharacterStateID::Jump);
}

void USmashCharacterStateRun::OnInputCrouch(float InputCrouch)
{
	if (InputCrouch < -0.1f)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Crouch);
	}
}

void USmashCharacterStateRun::StateEnter(ESmashCharacterStateID PreviousState)
{
	Super::StateEnter(PreviousState);

	Character->PlayAnimMontage(RunAnim);

	MovementComponent = Character->GetCharacterMovement();

	if (MovementComponent != nullptr)
		MovementComponent->MaxWalkSpeed = RunMoveSpeedMax;

	Character->InputJumpEvent.AddDynamic(this, &USmashCharacterStateRun::OnInputJump);
	Character->InputCrouchEvent.AddDynamic(this, &USmashCharacterStateRun::OnInputCrouch);
}

void USmashCharacterStateRun::StateExit(ESmashCharacterStateID NextState)
{
	Super::StateExit(NextState);

	Character->InputJumpEvent.RemoveDynamic(this, &USmashCharacterStateRun::OnInputJump);
	Character->InputCrouchEvent.RemoveDynamic(this, &USmashCharacterStateRun::OnInputCrouch);
}

void USmashCharacterStateRun::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	if (!Character->GetCharacterMovement()->IsMovingOnGround())
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Fall);
	}
	
	if (FMath::Abs(Character->GetInputMoveX()) < CharacterSettings->InputMoveXThreshold)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Idle);
	}
	else
	{
		Character->SetOrientX(Character->GetInputMoveX());
		Character->AddMovementInput(FVector::ForwardVector, Character->GetOrientX());
	}
}
