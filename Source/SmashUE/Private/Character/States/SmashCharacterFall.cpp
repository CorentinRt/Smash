// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/States/SmashCharacterFall.h"

#include "Character/SmashCharacter.h"
#include "Character/SmashCharacterStateID.h"
#include "Character/SmashCharacterStateMachine.h"
#include "GameFramework/PawnMovementComponent.h"


ESmashCharacterStateID USmashCharacterFall::GetStateID()
{
	return ESmashCharacterStateID::Fall;
}

void USmashCharacterFall::StateEnter(ESmashCharacterStateID PreviousState)
{
	Super::StateEnter(PreviousState);

	Character->PlayAnimMontage(FallAnim);
}

void USmashCharacterFall::StateExit(ESmashCharacterStateID NextState)
{
	Super::StateExit(NextState);
}

void USmashCharacterFall::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	Character->SetOrientX(Character->GetInputMoveX());
	Character->AddMovementInput(FVector::ForwardVector, Character->GetOrientX());
	
	if (Character->GetMovementComponent()->IsMovingOnGround())
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Idle);
	}
}
