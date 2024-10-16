// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/States/SmashCharacterStateIdle.h"

#include "Character/SmashCharacter.h"
#include "Character/SmashCharacterSettings.h"
#include "Character/SmashCharacterStateID.h"
#include "Character/SmashCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"

ESmashCharacterStateID USmashCharacterStateIdle::GetStateID()
{
	return ESmashCharacterStateID::Idle;
}

void USmashCharacterStateIdle::OnInputJump(float InputMoveX)
{
	StateMachine->ChangeState(ESmashCharacterStateID::Jump);
}

void USmashCharacterStateIdle::OnInputCrouch(float InputCrouch)
{
	if (InputCrouch < -0.1f)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Crouch);
	}
}

void USmashCharacterStateIdle::StateEnter(ESmashCharacterStateID PreviousState)
{
	Super::StateEnter(PreviousState);

	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("Idle"));
	
	Character->PlayAnimMontage(IdleAnim);

	Character->InputMoveXFastEvent.AddDynamic(this, &USmashCharacterStateIdle::OnInputMoveXFast);
	Character->InputJumpEvent.AddDynamic(this, &USmashCharacterStateIdle::OnInputJump);
	Character->InputMoveYEvent.AddDynamic(this, &USmashCharacterStateIdle::OnInputCrouch);
}

void USmashCharacterStateIdle::OnInputMoveXFast(float InputMoveX)
{
	StateMachine->ChangeState(ESmashCharacterStateID::Run);
}

void USmashCharacterStateIdle::StateExit(ESmashCharacterStateID NextState)
{
	Super::StateExit(NextState);
	
	Character->InputMoveXFastEvent.RemoveDynamic(this, &USmashCharacterStateIdle::OnInputMoveXFast);
	Character->InputJumpEvent.RemoveDynamic(this, &USmashCharacterStateIdle::OnInputJump);
	Character->InputMoveYEvent.RemoveDynamic(this, &USmashCharacterStateIdle::OnInputCrouch);
}

void USmashCharacterStateIdle::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	
	if (!Character->GetCharacterMovement()->IsMovingOnGround())
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Fall);
	}

	if (FMath::Abs(Character->GetInputMoveX()) > CharacterSettings->InputMoveXThreshold)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Walk);
	}
}



