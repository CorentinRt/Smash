// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/States/SmashCharacterStateJump.h"

#include "Character/SmashCharacter.h"
#include "Character/SmashCharacterStateMachine.h"


ESmashCharacterStateID USmashCharacterStateJump::GetStateID()
{
	return ESmashCharacterStateID::Jump;
}

void USmashCharacterStateJump::OnInputDoubleJump(float InputDoubleJump)
{
	StateMachine->ChangeState(ESmashCharacterStateID::DoubleJump);
}

void USmashCharacterStateJump::Jump()
{
	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Emerald,
		TEXT("Jump")
	);
	Character->Jump();
	Character->PlayAnimMontage(JumpAnim);
}

void USmashCharacterStateJump::StateEnter(ESmashCharacterStateID PreviousState)
{
	Super::StateEnter(PreviousState);

	Jump();

	Character->InputJumpEvent.AddDynamic(this, &USmashCharacterStateJump::OnInputDoubleJump);
}

void USmashCharacterStateJump::StateExit(ESmashCharacterStateID NextState)
{
	Super::StateExit(NextState);
	
	Character->InputJumpEvent.RemoveDynamic(this, &USmashCharacterStateJump::OnInputDoubleJump);
}

void USmashCharacterStateJump::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	Character->SetOrientX(Character->GetInputMoveX());
	Character->AddMovementInput(FVector::ForwardVector, Character->GetOrientX());
	
	if (Character->GetVelocity().Z <= 0.f)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Fall);
	}
}
