// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/CameraWorldSubsystem.h"

#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

void UCameraWorldSubsystem::PostInitialize()
{
	Super::PostInitialize();
}

void UCameraWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	CameraMain = FindCameraByTag(TEXT("CameraMain"));
}

void UCameraWorldSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickUpdateCameraPosition(DeltaTime);
}

void UCameraWorldSubsystem::AddFollowTarget(AActor* FollowTarget)
{
	if (FollowTarget == nullptr)	return;
	if (FollowTargets.Contains(FollowTarget))	return;

	FollowTargets.Add(FollowTarget);
}

void UCameraWorldSubsystem::RemoveFollowTarget(AActor* FollowTarget)
{
	if (FollowTarget == nullptr)	return;
	if (!FollowTargets.Contains(FollowTarget))	return;

	FollowTargets.Remove(FollowTarget);
}

void UCameraWorldSubsystem::TickUpdateCameraPosition(float DeltaTime)
{
	if (CameraMain == nullptr)	return;

	FVector AverageLocation = CalculateAveragePositionBetweenTargets();
	FVector CameraLocation = CameraMain->K2_GetComponentToWorld().GetLocation();
	AverageLocation.Y = CameraLocation.Y;
	CameraMain->SetWorldLocation(AverageLocation);
}

FVector UCameraWorldSubsystem::CalculateAveragePositionBetweenTargets()
{
	FVector TotalLocation = FVector::ZeroVector;

	int NbTargets = 0;
	
	for (AActor* Target : FollowTargets)
	{
		if (Target == nullptr)	continue;

		TotalLocation += Target->GetActorLocation();
		NbTargets++;
	}

	return TotalLocation/NbTargets;
}

UCameraComponent* UCameraWorldSubsystem::FindCameraByTag(const FName& Tag) const
{
	if (!GetWorld())	return nullptr;
	TArray<AActor*> CameraComponentActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), Tag, CameraComponentActors);
	
	
	for (AActor* Actor : CameraComponentActors)
	{
		UCameraComponent* CameraComponent = Actor->GetComponentByClass<UCameraComponent>();

		if (CameraComponent != nullptr)
			return CameraComponent;
	}

	return nullptr;
}
