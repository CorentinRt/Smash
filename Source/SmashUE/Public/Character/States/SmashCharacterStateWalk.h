﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/SmashCharacterState.h"
#include "SmashCharacterStateWalk.generated.h"


class UCharacterMovementComponent;

UCLASS(ClassGroup=(SmashCharacterState), meta=(BlueprintSpawnableComponent))
class SMASHUE_API USmashCharacterStateWalk : public USmashCharacterState
{
	GENERATED_BODY()

public:
	virtual ESmashCharacterStateID GetStateID() override;

	virtual void StateEnter(ESmashCharacterStateID PreviousState) override;

	virtual void StateExit(ESmashCharacterStateID NextState) override;

	virtual void StateTick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UAnimMontage* WalkAnim;

	UPROPERTY(EditAnywhere)
	float MoveSpeedMax = 250.f;

	UPROPERTY()
	UCharacterMovementComponent* MovementComponent;
};