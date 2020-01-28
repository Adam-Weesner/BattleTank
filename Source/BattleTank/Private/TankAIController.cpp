// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "TankAimingComponent.h"
#include "BattleTank.h"
#include "Tank.h" // Implements OnDeath()

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();

	// On player tank's death
	if (!ensure(GetWorld()->GetFirstPlayerController()->GetPawn())) { return; }
	auto playerTank = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (!ensure(playerTank)) { return; }
	playerTank->OnTankDeath.AddUniqueDynamic(this, &ATankAIController::OnPlayerTankDeath);
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


void ATankAIController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	// On own tank's death
	auto owningTank = Cast<ATank>(InPawn);
	if (!ensure(owningTank)) { return; }
	owningTank->OnTankDeath.AddUniqueDynamic(this, &ATankAIController::OnTankDeath);
}


void ATankAIController::OnTankDeath()
{
	UE_LOG(LogTemp, Error, TEXT("AI %s is dead!"), *GetPawn()->GetName());
	GetPawn()->DetachFromControllerPendingDestroy();
}

void ATankAIController::OnPlayerTankDeath()
{
	UE_LOG(LogTemp, Error, TEXT("Player's %s is dead!"), *GetPawn()->GetName());
}
