// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Tank.h"
#include "BattleTank.h"


void ATankAIController::BeginPlay()
{
	Super::BeginPlay();

	playerTank = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
	controlledTank = Cast<ATank>(GetPawn());
}

// Called every frame
void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!playerTank || !controlledTank) { return; }

	// TODO Move towards the player
		
	// Aim towards the player
	controlledTank->AimAt(playerTank->GetActorLocation());
	controlledTank->Fire();
}