﻿// Fill out your copyright notice in the Description page of Project Settings.


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

	if (CameraMain != nullptr)
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Orange, "CameraBoundsActor");
	
	AActor* CameraBoundsActor = FindCameraBoundsActor();
	if (CameraBoundsActor != nullptr)
	{
		InitCameraBounds(CameraBoundsActor);
	}

	InitCameraZoomParameters();
}

void UCameraWorldSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickUpdateCameraZoom(DeltaTime);
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
	ClampPositionIntoCameraBounds(AverageLocation);

	FVector NewCameraPosition = CameraMain->GetOwner()->GetActorLocation();
	NewCameraPosition.X = AverageLocation.X;
	NewCameraPosition.Z = AverageLocation.Z;
	CameraMain->GetOwner()->SetActorLocation(NewCameraPosition);
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

float UCameraWorldSubsystem::CalculateGreatestDistanceBetweenTargets()
{
	float GreatestDistance = 0.f;

	for (int i = 0; i < FollowTargets.Num(); i++)
	{
		ICameraFollowTarget* CameraFollowTargetOne = Cast<ICameraFollowTarget>(FollowTargets[i]);
		if (CameraFollowTargetOne == nullptr)	continue;
		
		for (int j = 0; j < FollowTargets.Num(); j++)
		{
			ICameraFollowTarget* CameraFollowTargetTwo = Cast<ICameraFollowTarget>(FollowTargets[j]);
			if (CameraFollowTargetTwo == nullptr)	continue;
			
			float CurrentDistance = (CameraFollowTargetOne->GetFollowPosition() - CameraFollowTargetTwo->GetFollowPosition()).Length();

			if (CurrentDistance > GreatestDistance)
			{
				GreatestDistance = CurrentDistance;
			}
		}
	}
	
	return GreatestDistance;
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

void UCameraWorldSubsystem::TickUpdateCameraZoom(float DeltaTime)
{
	if (CameraMain == nullptr)	return;

	float GreatestDistanceBetweenPlayers = CalculateGreatestDistanceBetweenTargets();

	float Percent = GreatestDistanceBetweenPlayers / (FMath::Abs(CameraZoomDistanceBetweenTargetsMax - CameraZoomDistanceBetweenTargetsMin) * 10.f);

	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, FString::Printf(TEXT("Percent : %f"), Percent));
	Percent = FMath::Clamp(Percent, 0.f, 1.f);

	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, FString::Printf(TEXT("Greatest distance : %f"), GreatestDistanceBetweenPlayers));
	
	float InversePercent = 1.f - Percent;

	FVector CameraPos = CameraMain->GetOwner()->GetActorLocation();

	float YPos = FMath::Lerp(CameraZoomYMin, CameraZoomYMax, InversePercent);
	
	CameraMain->GetOwner()->SetActorLocation(FVector(CameraPos.X, YPos, CameraPos.Z));
}

AActor* UCameraWorldSubsystem::FindCameraBoundsActor()
{
	TArray<AActor*> CameraBoundsActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "CameraBounds", CameraBoundsActors);

	for (AActor* Actor : CameraBoundsActors)
	{
		return Actor;
	}

	return nullptr;
}

void UCameraWorldSubsystem::InitCameraBounds(AActor* CameraBoundsActor)
{
	FVector BoundsCenter;
	FVector BoundsExtents;
	CameraBoundsActor->GetActorBounds(false, BoundsCenter, BoundsExtents);

	CameraBoundsMin = FVector2D(BoundsCenter.X - BoundsExtents.X, BoundsCenter.Z - BoundsExtents.Z);
	CameraBoundsMax = FVector2D(BoundsCenter.X + BoundsExtents.X, BoundsCenter.Z + BoundsExtents.Z);

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("CameraBoundsMin: X=%f, Z=%f"), CameraBoundsMin.X, CameraBoundsMin.Y));
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("CameraBoundsMin: X=%f, Z=%f"), CameraBoundsMax.X, CameraBoundsMax.Y));

	
	CameraBoundsYProjectionCenter = BoundsCenter.Y;
}

void UCameraWorldSubsystem::ClampPositionIntoCameraBounds(FVector& Position)
{
	FVector2D ViewportBoundsMin, ViewportBoundsMax;
	GetViewportBounds(ViewportBoundsMin, ViewportBoundsMax);

	FVector WorldBoundsMin = CalculateWorldPositionFromViewportPosition(ViewportBoundsMin);
	FVector WorldBoundsMax = CalculateWorldPositionFromViewportPosition(ViewportBoundsMax);

	//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Purple, FString::Printf(TEXT("WorldBoundsMin: X=%f, Z=%f"), WorldBoundsMin.X, WorldBoundsMin.Z));
	//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Purple, FString::Printf(TEXT("WorldBoundsMax: X=%f, Z=%f"), WorldBoundsMax.X, WorldBoundsMax.Z));

	FVector WorldBoundsRange = WorldBoundsMax - WorldBoundsMin;
	FVector WorldBoundsExtents = WorldBoundsRange / 2.f;
	
	Position.X = FMath::Clamp(Position.X, CameraBoundsMin.X + WorldBoundsExtents.X, CameraBoundsMax.X - WorldBoundsExtents.X);
	Position.Z = FMath::Clamp(Position.Z, CameraBoundsMin.Y - WorldBoundsExtents.Z, CameraBoundsMax.Y + WorldBoundsExtents.Z);
}

void UCameraWorldSubsystem::GetViewportBounds(FVector2D& OutViewportBoundsMin, FVector2D& OutViewportBoundsMax)
{
	// Find Viewport
	UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
	if (ViewportClient == nullptr)	return;

	FViewport* Viewport = ViewportClient->Viewport;
	if (Viewport == nullptr)	return;

	// Calculate Viewport Rect according to Camera Aspect Ratio and Viewport ViewRect
	FIntRect ViewRect(
		Viewport->GetInitialPositionXY(),
		Viewport->GetInitialPositionXY() + Viewport->GetSizeXY()
	);
	FIntRect ViewportRect = Viewport->CalculateViewExtents(CameraMain->AspectRatio, ViewRect);

	// Fill Output parameters with ViewportRect
	OutViewportBoundsMin.X = ViewportRect.Min.X;
	OutViewportBoundsMin.Y = ViewportRect.Min.Y;

	OutViewportBoundsMax.X = ViewportRect.Max.X;
	OutViewportBoundsMax.Y = ViewportRect.Max.Y;
}

FVector UCameraWorldSubsystem::CalculateWorldPositionFromViewportPosition(const FVector2D& ViewportPosition)
{
	if (CameraMain == nullptr)	return FVector::ZeroVector;

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController == nullptr)	return FVector::ZeroVector;

	float YDistanceToCenter = CameraMain->GetOwner()->GetActorLocation().Y - CameraBoundsYProjectionCenter;

	FVector CameraWorldProjectDir;
	FVector WorldPosition;
	UGameplayStatics::DeprojectScreenToWorld(
		PlayerController,
		ViewportPosition,
		WorldPosition,
		CameraWorldProjectDir
	);

	WorldPosition += CameraWorldProjectDir * YDistanceToCenter;

	return WorldPosition;
}

void UCameraWorldSubsystem::InitCameraZoomParameters()
{
	UCameraComponent* CameraDistanceMin = FindCameraByTag(TEXT("CameraDistanceMin"));

	if (CameraDistanceMin != nullptr)
	{
		CameraZoomYMin = CameraDistanceMin->GetOwner()->GetActorLocation().Y;
	}

	UCameraComponent* CameraDistanceMax = FindCameraByTag(TEXT("CameraDistanceMax"));

	if (CameraDistanceMax != nullptr)
	{
		CameraZoomYMin = CameraDistanceMax->GetOwner()->GetActorLocation().Y;
	}
}
