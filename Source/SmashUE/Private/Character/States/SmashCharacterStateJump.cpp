﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/States/SmashCharacterStateJump.h"

#include "Character/SmashCharacter.h"
#include "Character/SmashCharacterStateMachine.h"


ESmashCharacterStateID USmashCharacterStateJump::GetStateID()
{
	return ESmashCharacterStateID::Jump;
}

void USmashCharacterStateJump::OnInputJump(float InputMoveX)
{
	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Emerald,
		TEXT("Jump")
	);
	Character->Jump();
}

void USmashCharacterStateJump::StateEnter(ESmashCharacterStateID PreviousState)
{
	Super::StateEnter(PreviousState);
	
	Character->PlayAnimMontage(JumpAnim);

	Character->InputJumpEvent.AddDynamic(this, &USmashCharacterStateJump::OnInputJump);
}

void USmashCharacterStateJump::StateExit(ESmashCharacterStateID NextState)
{
	Super::StateExit(NextState);
	
	Character->InputJumpEvent.RemoveDynamic(this, &USmashCharacterStateJump::OnInputJump);
}

void USmashCharacterStateJump::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	if (Character->GetVelocity().Z < 0.f)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Fall);
	}
}