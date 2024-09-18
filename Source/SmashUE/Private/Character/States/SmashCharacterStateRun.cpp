// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/States/SmashCharacterStateRun.h"
#include "Character/SmashCharacter.h"
#include "Character/SmashCharacterStateID.h"
#include "GameFramework/CharacterMovementComponent.h"


ESmashCharacterStateID USmashCharacterStateRun::GetStateID()
{
	return ESmashCharacterStateID::Run;
}

void USmashCharacterStateRun::StateEnter(ESmashCharacterStateID PreviousState)
{
	Super::StateEnter(PreviousState);
	
	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Cyan,
		TEXT("Enter State Run")
	);

	Character->PlayAnimMontage(RunAnim);

	MovementComponent = Character->GetCharacterMovement();

	if (MovementComponent != nullptr)
		MovementComponent->MaxWalkSpeed = RunMoveSpeedMax;
}

void USmashCharacterStateRun::StateExit(ESmashCharacterStateID NextState)
{
	Super::StateExit(NextState);

	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Red,
		TEXT("Exit State Run")
	);

	Character->StopAnimMontage(RunAnim);
}

void USmashCharacterStateRun::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	
	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Emerald,
		TEXT("Tick State Run")
		
	);
	if (MovementComponent != nullptr)
		MovementComponent->AddInputVector(Character->GetActorForwardVector() * Character->GetOrientX());
}
