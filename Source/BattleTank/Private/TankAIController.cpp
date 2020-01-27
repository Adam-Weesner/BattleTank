// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "TankAimingComponent.h"
#include "BattleTank.h"


void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto playerTank = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!ensure(playerTank)) { return; }

	// Move towards the player
	MoveToActor(playerTank, acceptanceRadius); // TODO Check radius is in cm
		
	// Aim towards the player
	auto aimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	aimingComponent->AimAt(playerTank->GetActorLocation());

	if (aimingComponent->GerFiringState() == EFiringStatus::Locked)
	{
		aimingComponent->Fire();
	}
}