// Fill out your copyright notice in the Description page of Project Settings.


#include "Match/MatchGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "Arena/ArenaPlayerStart.h"

void AMatchGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AArenaPlayerStart*> PlayerStartsPoints;
	FindPlayerStartActorsInArea(PlayerStartsPoints);

	for (AArenaPlayerStart* PlayerStartPoint : PlayerStartsPoints)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			5.f,
			FColor::Red,
			PlayerStartPoint->GetFName().ToString()
			);
	}
}

void AMatchGameMode::FindPlayerStartActorsInArea(TArray<AArenaPlayerStart*>& ResultsActors)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AArenaPlayerStart::StaticClass(), FoundActors);

	for (int i = 0; i < FoundActors.Num(); ++i)
	{
		AArenaPlayerStart* ArenaPlayerStart = Cast<AArenaPlayerStart>(FoundActors[i]);

		if (ArenaPlayerStart == nullptr) continue;

		ResultsActors.Add(ArenaPlayerStart);
	}
}
