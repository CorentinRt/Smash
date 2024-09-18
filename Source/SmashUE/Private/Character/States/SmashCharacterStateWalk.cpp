// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/States/SmashCharacterStateWalk.h"

#include "Character/SmashCharacter.h"
#include "Character/SmashCharacterStateID.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/SmashCharacterStateMachine.h"
#include "Character/SmashCharacterSettings.h"

ESmashCharacterStateID USmashCharacterStateWalk::GetStateID()
{
	return ESmashCharacterStateID::Walk;
}

void USmashCharacterStateWalk::OnInputJump(float InputMoveX)
{
	StateMachine->ChangeState(ESmashCharacterStateID::Jump);
}
void USmashCharacterStateWalk::OnInputMoveXFast(float InputMoveX)
{
	StateMachine->ChangeState(ESmashCharacterStateID::Run);
}

void USmashCharacterStateWalk::StateEnter(ESmashCharacterStateID PreviousState)
{
	Super::StateEnter(PreviousState);

	Character->PlayAnimMontage(WalkAnim);

	MovementComponent = Character->GetCharacterMovement();

	if (MovementComponent != nullptr)
		MovementComponent->MaxWalkSpeed = MoveSpeedMax;

	Character->InputMoveXFastEvent.AddDynamic(this, &USmashCharacterStateWalk::OnInputMoveXFast);
	Character->InputJumpEvent.AddDynamic(this, &USmashCharacterStateWalk::OnInputJump);
}


void USmashCharacterStateWalk::StateExit(ESmashCharacterStateID NextState)
{
	Super::StateExit(NextState);
	
	Character->InputMoveXFastEvent.RemoveDynamic(this, &USmashCharacterStateWalk::OnInputMoveXFast);
	Character->InputJumpEvent.AddDynamic(this, &USmashCharacterStateWalk::OnInputJump);
}

void USmashCharacterStateWalk::StateTick(float DeltaTime)
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
