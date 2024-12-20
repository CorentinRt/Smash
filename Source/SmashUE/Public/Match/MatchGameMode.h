﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MatchGameMode.generated.h"

class UInputMappingContext;
class USmashCharacterInputData;
class ASmashCharacter;
class AArenaPlayerStart;

/**
 * 
 */
UCLASS()
class SMASHUE_API AMatchGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
private:
	USmashCharacterInputData* LoadInputDataFromConfig();

	UInputMappingContext* LoadInputMappingContextFromConfig();
	
	void FindPlayerStartActorsInArea(TArray<AArenaPlayerStart*> &ResultsActors);

	void SpawnCharacters(const TArray<AArenaPlayerStart*> SpawnPoints);
	
	TSubclassOf<ASmashCharacter> GetSmashCharacterClassFromInputType(EAutoReceiveInput::Type InputType) const;

	void CreateAndInitPlayers() const;

protected:
	UPROPERTY(EditAnywhere)
	TArray<ASmashCharacter*> CharactersInsideArena;
};
