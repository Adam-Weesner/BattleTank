// Fill out your copyright notice in the Description page of Project Settings.


#include "TankBarrel.h"

void UTankBarrel::Elevate(float relativeSpeed)
{
	relativeSpeed = FMath::Clamp<float>(relativeSpeed, -1, 1);
	auto elevationChange = relativeSpeed * maxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	auto rawNewElevation = GetRelativeRotation().Pitch + elevationChange;

	SetRelativeRotation(FRotator(FMath::Clamp<float>(rawNewElevation, minElevationDegrees, maxElevationDegrees), 0, 0));
}
