// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "TankAIController.generated.h"

UCLASS()
class BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()

protected:
	// How close can AI tank get
	UPROPERTY(EditAnywhere, Category = "Setup")
	float acceptanceRadius = 3000.0f;
	
private:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
};
