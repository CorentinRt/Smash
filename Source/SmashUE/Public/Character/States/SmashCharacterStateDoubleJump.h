// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/SmashCharacterState.h"
#include "SmashCharacterStateDoubleJump.generated.h"


UCLASS(ClassGroup=(SmashCharacterState), meta=(BlueprintSpawnableComponent))
class SMASHUE_API USmashCharacterStateDoubleJump : public USmashCharacterState
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void DoubleJump();
	
	virtual ESmashCharacterStateID GetStateID();

	virtual void StateEnter(ESmashCharacterStateID PreviousState) override;
	
	virtual void StateExit(ESmashCharacterStateID NextState) override;

	virtual void StateTick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UAnimMontage* DoubleJumpAnim;
};
