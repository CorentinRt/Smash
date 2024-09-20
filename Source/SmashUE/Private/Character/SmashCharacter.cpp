// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SmashCharacter.h"
#include "Character/SmashCharacterStateMachine.h"
#include "EnhancedInputSubsystems.h"
#include "Character/SmashCharacterInputData.h"
#include "EnhancedInputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/DynamicMeshComponent.h"

void ASmashCharacter::OnCollisionEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr) return;
	
	if (OtherActor->Tags.Contains("OneWayPlatform"))
	{
		GetCapsuleComponent()->IgnoreComponentWhenMoving(OtherActor->GetComponentByClass<UDynamicMeshComponent>(), true);
	}
}

void ASmashCharacter::OnCollisionExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == nullptr) return;
	
	if (OtherActor->Tags.Contains("OneWayPlatform"))
	{
		GetCapsuleComponent()->IgnoreComponentWhenMoving(OtherActor->GetComponentByClass<UDynamicMeshComponent>(), false);
	}
}

// Sets default values
ASmashCharacter::ASmashCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ASmashCharacter::OnCollisionEnter);

		GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &ASmashCharacter::OnCollisionExit);
	}
    
}

// Called when the game starts or when spawned
void ASmashCharacter::BeginPlay()
{
	Super::BeginPlay();
	CreateStateMachine();
	InitStateMachine();
}

// Called every frame
void ASmashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickStateMachine(DeltaTime);
	RotateMeshUsingOrientX();
}

// Called to bind functionality to input
void ASmashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	SetupMappingContextIntoController();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent == nullptr) return;
	
	BindInputMoveXAxisAndActions(EnhancedInputComponent);
	BindInputMoveXFastAndActions(EnhancedInputComponent);
	BindInputJumpAndActions(EnhancedInputComponent);
	BindInputMoveYAndActions(EnhancedInputComponent);
}

float ASmashCharacter::GetOrientX() const
{
	return OrientX;
}

void ASmashCharacter::SetOrientX(float NewOrientX)
{
	OrientX = NewOrientX;
}

void ASmashCharacter::RotateMeshUsingOrientX() const
{
	FRotator Rotation = GetMesh()->GetRelativeRotation();

	Rotation.Yaw = -90.f * OrientX;

	GetMesh()->SetRelativeRotation(Rotation);
}

void ASmashCharacter::CreateStateMachine()
{
	StateMachine = NewObject<USmashCharacterStateMachine>(this);
}

void ASmashCharacter::InitStateMachine()
{
	if (StateMachine == nullptr) return;

	StateMachine->Init(this);
}

void ASmashCharacter::TickStateMachine(float DeltaTime) const
{
	if (StateMachine == nullptr) return;

	StateMachine->Tick(DeltaTime);
}

void ASmashCharacter::SetupMappingContextIntoController() const
{
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController == nullptr) return;

	ULocalPlayer* Player = PlayerController->GetLocalPlayer();
	if (Player == nullptr) return;

	UEnhancedInputLocalPlayerSubsystem* InputSystem = Player->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (InputSystem == nullptr) return;

	InputSystem->AddMappingContext(InputMappingContext, 0);
	
}

float ASmashCharacter::GetInputMoveX() const
{
	return InputMoveX;
}

void ASmashCharacter::OnInputMoveX(const FInputActionValue& InputActionValue)
{
	InputMoveX = InputActionValue.Get<float>();
}

void ASmashCharacter::OnInputMoveXFast(const FInputActionValue& InputActionValue)
{
	InputMoveX = InputActionValue.Get<float>();
	InputMoveXFastEvent.Broadcast(InputMoveX);
}

void ASmashCharacter::BindInputJumpAndActions(UEnhancedInputComponent* EnhancedInputComponent)
{
	if (InputData == nullptr) return;
	
#pragma region Bind Input Jump
	if (InputData->InputActionJump)
	{
		EnhancedInputComponent->BindAction(
			InputData->InputActionJump,
			ETriggerEvent::Started,
			this,
			&ASmashCharacter::OnInputJump
		);
	}
#pragma endregion
}

void ASmashCharacter::OnInputJump(const FInputActionValue& InputActionValue)
{
	InputJumpValue = InputActionValue.Get<float>();
	InputJumpEvent.Broadcast(InputJumpValue);
}

float ASmashCharacter::GetInputMoveYValue() const
{
	return InputMoveYValue;
}

void ASmashCharacter::CheckOneWayFloor()
{
	FVector Start = GetActorLocation();
	FVector End = Start - FVector(0, 0, 100.f);  // Vérifie à 500 unités sous le personnage

	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("Trace")), true, this);
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		Hit,
		Start,
		End,
		ECC_Visibility,
		TraceParams
	);

	if (bHit && Hit.GetActor())
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("Objet sous le personnage: %s"), *HitActor->GetName());

			if (HitActor->Tags.Contains("OneWayPlatform"))
			{

				GEngine->AddOnScreenDebugMessage(
					-1,
					3.f,
					FColor::Red,
					TEXT("One Way Platform")
				);
				
				GetCapsuleComponent()->IgnoreComponentWhenMoving(HitActor->GetComponentByClass<UDynamicMeshComponent>(), true);
			}
		}
	}
}

void ASmashCharacter::BindInputMoveYAndActions(UEnhancedInputComponent* EnhancedInputComponent)
{
	if (InputData == nullptr) return;
	
#pragma region Bind Input Move Y

	if (InputData->InputActionMoveY)
	{
		EnhancedInputComponent->BindAction(
			InputData->InputActionMoveY,
			ETriggerEvent::Started,
			this,
			&ASmashCharacter::OnInputMoveY
		);

		EnhancedInputComponent->BindAction(
			InputData->InputActionMoveY,
			ETriggerEvent::Triggered,
			this,
			&ASmashCharacter::OnInputMoveY
		);
		
		EnhancedInputComponent->BindAction(
			InputData->InputActionMoveY,
			ETriggerEvent::Completed,
			this,
			&ASmashCharacter::OnInputMoveY
		);

		EnhancedInputComponent->BindAction(
			InputData->InputActionMoveYFast,
			ETriggerEvent::Triggered,
			this,
			&ASmashCharacter::OnInputMoveYFast
		);
	}

#pragma endregion 
}

void ASmashCharacter::OnInputMoveY(const FInputActionValue& InputActionValue)
{
	InputMoveYValue = InputActionValue.Get<float>();
	InputMoveYEvent.Broadcast(InputMoveYValue);
}

void ASmashCharacter::OnInputMoveYFast(const FInputActionValue& InputActionValue)
{
	InputMoveYValue = InputActionValue.Get<float>();
	InputMoveYFastEvent.Broadcast(InputMoveYValue);
	
	CheckOneWayFloor();
}

void ASmashCharacter::BindInputMoveXAxisAndActions(UEnhancedInputComponent* EnhancedInputComponent)
{
	if (InputData == nullptr) return;

#pragma region Bind Input Move X
	if (InputData->InputActionMoveX)
	{
		EnhancedInputComponent->BindAction(
			InputData->InputActionMoveX,
			ETriggerEvent::Started,
			this,
			&ASmashCharacter::OnInputMoveX
		);

		EnhancedInputComponent->BindAction(
			InputData->InputActionMoveX,
			ETriggerEvent::Completed,
			this,
			&ASmashCharacter::OnInputMoveX
		);

		EnhancedInputComponent->BindAction(
			InputData->InputActionMoveX,
			ETriggerEvent::Triggered,
			this,
			&ASmashCharacter::OnInputMoveX
		);
	}
#pragma endregion
	
}

void ASmashCharacter::BindInputMoveXFastAndActions(UEnhancedInputComponent* EnhancedInputComponent)
{
	if (InputData == nullptr) return;
	
#pragma region Bind Input MoveFast
	if (InputData->InputActionMoveXFast)
	{
		EnhancedInputComponent->BindAction(
			InputData->InputActionMoveXFast,
			ETriggerEvent::Triggered,
			this,
			&ASmashCharacter::OnInputMoveXFast
		);
	}
#pragma endregion
}



