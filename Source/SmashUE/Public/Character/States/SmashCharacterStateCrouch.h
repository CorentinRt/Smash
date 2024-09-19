// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/SmashCharacterState.h"
#include "SmashCharacterStateCrouch.generated.h"


UCLASS(ClassGroup=(SmashCharacterState), meta=(BlueprintSpawnableComponent))
class SMASHUE_API USmashCharacterStateCrouch : public USmashCharacterState
{
	GENERATED_BODY()

public:
	virtual ESmashCharacterStateID GetStateID() override;

	UFUNCTION()
	void OnInputJump(float InputMoveX);
	
	virtual void StateEnter(ESmashCharacterStateID PreviousState) override;

	virtual void StateExit(ESmashCharacterStateID NextState) override;

	virtual void StateTick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UAnimMontage* CrouchAnim;

	UPROPERTY(EditAnywhere)
	float MoveCrouchSpeedMax = 150.f;

	
};
