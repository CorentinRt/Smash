// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/States/SmashCharacterStateDoubleJump.h"

#include "Character/SmashCharacter.h"
#include "Character/SmashCharacterStateID.h"
#include "Character/SmashCharacterStateMachine.h"


void USmashCharacterStateDoubleJump::DoubleJump()
{
	Character->PlayAnimMontage(DoubleJumpAnim);
	Character->Jump();
}

ESmashCharacterStateID USmashCharacterStateDoubleJump::GetStateID()
{
	return ESmashCharacterStateID::DoubleJump;
}

void USmashCharacterStateDoubleJump::StateEnter(ESmashCharacterStateID PreviousState)
{
	Super::StateEnter(PreviousState);
	
	DoubleJump();
}

void USmashCharacterStateDoubleJump::StateExit(ESmashCharacterStateID NextState)
{
	Super::StateExit(NextState);

	
}

void USmashCharacterStateDoubleJump::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	
	if (Character->GetVelocity().Z <= 0.f)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Fall);
	}
}
