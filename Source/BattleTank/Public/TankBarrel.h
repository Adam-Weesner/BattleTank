// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankBarrel.generated.h"

UCLASS(meta = (BlueprintSpawnableComponent))//, hideCategories = ("Collision"))
class BATTLETANK_API UTankBarrel : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	// -1 is max downward speed, +1 is max up movement
	void Elevate(float relativeSpeed);
	
private:
	UPROPERTY(EditDefaultsOnly, Category = Setup)
	float maxDegreesPerSecond = 10.0f;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
	float maxElevationDegrees = 40.0f;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
	float minElevationDegrees = 0.0f;
};
