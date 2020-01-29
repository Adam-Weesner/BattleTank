// Fill out your copyright notice in the Description page of Project Settings.


#include "TankTrack.h"
#include "SprungWheel.h"
#include "SpawnPoint.h"

UTankTrack::UTankTrack()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTankTrack::SetThrottle(float throttle)
{
	float currentThrottle = FMath::Clamp<float>(throttle, -1.0f, 1.0f);
	DriveTrack(currentThrottle);
}


void UTankTrack::DriveTrack(float CurrentThrottle)
{
	auto forceApplied = CurrentThrottle * trackMaxDrivingForce;
	auto Wheels = GetWheels();
	auto ForcePerWheel = forceApplied / Wheels.Num();

	for (ASprungWheel* Wheel : Wheels)
	{
		Wheel->AddDrivingForce(ForcePerWheel);
	}
}

TArray<ASprungWheel*> UTankTrack::GetWheels() const
{
	TArray<ASprungWheel*> SprungWheels;
	TArray<USceneComponent*> Children;

	GetChildrenComponents(true, Children);

	for (USceneComponent* Child : Children)
	{
		auto* SpawnPointChild = Cast<USpawnPoint>(Child);

		if (!SpawnPointChild) continue; // Continue doesn't exit for loop, but instead continues to the next child

		AActor* SpawnedChild = SpawnPointChild->GetSpawnedActor();
		auto SprungWheel = Cast<ASprungWheel>(SpawnedChild);

		if (!SprungWheel) continue;

		SprungWheels.Add(SprungWheel);
	}

	return SprungWheels;
}
