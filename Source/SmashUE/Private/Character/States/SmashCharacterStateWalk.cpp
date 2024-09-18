// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/States/SmashCharacterStateWalk.h"

#include "Character/SmashCharacter.h"
#include "Character/SmashCharacterStateID.h"



ESmashCharacterStateID USmashCharacterStateWalk::GetStateID()
{
	return ESmashCharacterStateID::Walk;
}

void USmashCharacterStateWalk::StateEnter(ESmashCharacterStateID PreviousState)
{
	Super::StateEnter(PreviousState);

	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Cyan,
		TEXT("Enter State Walk")
	);

	Character->PlayAnimMontage(WalkAnim);
}

void USmashCharacterStateWalk::StateExit(ESmashCharacterStateID NextState)
{
	Super::StateExit(NextState);

	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Red,
		TEXT("Exit State Walk")
	);

	Character->StopAnimMontage(WalkAnim);
}

void USmashCharacterStateWalk::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Emerald,
		TEXT("Tick State Walk")
	);
}
