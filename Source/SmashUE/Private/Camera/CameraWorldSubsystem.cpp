// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/CameraWorldSubsystem.h"

#include "Camera/CameraComponent.h"
#include "Camera/CameraFollowTarget.h"
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

void UCameraWorldSubsystem::AddFollowTarget(UObject* FollowTarget)
{
	if (FollowTarget == nullptr)	return;
	if (FollowTargets.Contains(FollowTarget))	return;

	FollowTargets.Add(FollowTarget);
}

void UCameraWorldSubsystem::RemoveFollowTarget(UObject* FollowTarget)
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
	
	for (UObject* Target : FollowTargets)
	{
		if (Target == nullptr)	continue;

		ICameraFollowTarget* CameraFollowTarget = Cast<ICameraFollowTarget>(Target);

		if (CameraFollowTarget == nullptr)	continue;
		
		if (!CameraFollowTarget->IsFollowable()) continue;
		
		TotalLocation += CameraFollowTarget->GetFollowPosition();
		NbTargets++;
	}

	if (NbTargets != 0)
	{
		return TotalLocation/NbTargets;
	}
	return TotalLocation;
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
