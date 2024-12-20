﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SmashCharacterState.h"

#include "Character/SmashCharacterSettings.h"
#include "Character/SmashCharacterStateID.h"
#include "Character/SmashCharacterStateMachine.h"

// Sets default values for this component's properties
USmashCharacterState::USmashCharacterState()
{
	PrimaryComponentTick.bCanEverTick = false;

}

ESmashCharacterStateID USmashCharacterState::GetStateID()
{
	return ESmashCharacterStateID::None;
}

void USmashCharacterState::StateInit(USmashCharacterStateMachine* InStateMachine)
{
	StateMachine = InStateMachine;

	Character = InStateMachine->GetCharacter();

	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Magenta,
		FString::Printf(TEXT("Init State %d"), GetStateID())
	);

	CharacterSettings = GetDefault<USmashCharacterSettings>();
}

void USmashCharacterState::StateEnter(ESmashCharacterStateID PreviousState)
{
	
}

void USmashCharacterState::StateExit(ESmashCharacterStateID NextState)
{
	
}

void USmashCharacterState::StateTick(float DeltaTime)
{
	
}


