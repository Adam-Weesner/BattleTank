// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankTrack.generated.h"

/*
 * TankTrack is used to set maximum driving force, and to apply forces to the tank
*/
UCLASS(meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankTrack : public UStaticMeshComponent
{
	GENERATED_BODY()

public: 
	// Set a throttle between -1 and 1
	UFUNCTION(BlueprintCallable, Category = "Input")
	void SetThrottle(float throttle);
	
	// Max force per track in newtons
	UPROPERTY(EditDefaultsOnly)
	float trackMaxDrivingForce = 14000000.0; // Assume 40 ton tank, and 1g acceleration

private:
	UTankTrack();

	void DriveTrack(float CurrentThrottle);

	TArray<class ASprungWheel*> GetWheels() const;
};
