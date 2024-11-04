// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Camera/CameraFollowTarget.h"
#include "GameFramework/Character.h"
#include "SmashCharacter.generated.h"

class USmashCharacterInputData;
class UInputMappingContext;
class USmashCharacterStateMachine;

UCLASS()
class SMASHUE_API ASmashCharacter : public ACharacter, public ICameraFollowTarget
{
	GENERATED_BODY()
	
#pragma region Unreal Defaults
public:
	UFUNCTION()
	void OnCollisionEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnCollisionExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Sets default values for this character's properties
	ASmashCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
#pragma endregion

#pragma region Orient
public:
	float GetOrientX() const;
	
	void SetOrientX(float NewOrientX);

protected:
	UPROPERTY(BlueprintReadOnly)
	float OrientX = 1.f;

	void RotateMeshUsingOrientX() const;
#pragma endregion

	
#pragma region State Machine	
public:
	void CreateStateMachine();

	void InitStateMachine();

	void TickStateMachine(float DeltaTime) const;

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USmashCharacterStateMachine> StateMachine;
#pragma endregion

#pragma region Input Data / Mapping Context

public:
	UPROPERTY()
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY()
	TObjectPtr<USmashCharacterInputData> InputData;

protected:
	void SetupMappingContextIntoController() const;

#pragma endregion

#pragma region Input Move

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputMoveXEvent, float, InputMoveX);
	
public:
	float GetInputMoveX() const;

	UPROPERTY()
	FInputMoveXEvent InputMoveXFastEvent;

protected:
	UPROPERTY()
	float InputMoveX = 0.f;

private:
	void OnInputMoveX(const FInputActionValue& InputActionValue);
	void BindInputMoveXAxisAndActions(UEnhancedInputComponent* EnhancedInputComponent);
#pragma endregion

#pragma region Input Move X Fast
	void OnInputMoveXFast(const FInputActionValue& InputActionValue);
	void BindInputMoveXFastAndActions(UEnhancedInputComponent* EnhancedInputComponent);
#pragma endregion
	
#pragma region Input Jump

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputJumpEvent, float, InputJumpValue);
public:
	UPROPERTY()
	FInputJumpEvent InputJumpEvent;

protected:
	UPROPERTY()
	float InputJumpValue = 0.f;

private:
	void BindInputJumpAndActions(UEnhancedInputComponent* EnhancedInputComponent);
	void OnInputJump(const FInputActionValue& InputActionValue);
#pragma endregion

#pragma region Input Move Y

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputMoveYEvent, float, InputMoveYValue);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputMoveYFastEvent, float, InputMoveYFastValue);
	
public:
	FInputMoveYEvent InputMoveYEvent;

	FInputMoveYFastEvent InputMoveYFastEvent;
	
	float GetInputMoveYValue() const;
	
protected:
	float InputMoveYValue = 0.f;

	UFUNCTION()
	void CheckOneWayFloor();
	
private:
	void BindInputMoveYAndActions(UEnhancedInputComponent* EnhancedInputComponent);
	void OnInputMoveY(const FInputActionValue& InputActionValue);
	void OnInputMoveYFast(const FInputActionValue& InputActionValue);

#pragma endregion

#pragma region Camera Target
public:
	virtual FVector GetFollowPosition() const override;
	virtual bool IsFollowable() const override;

private:
	
#pragma endregion 
	
};
