// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h"
#include "BattleTank.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UTankAimingComponent::Initialize(UTankBarrel* barrelToSet, UTankTurret* turretToSet)
{
	barrel = barrelToSet;
	turret = turretToSet;
}

EFiringStatus UTankAimingComponent::GerFiringState() const
{
	return firingState;
}

void UTankAimingComponent::AimAt(FVector HitLocation)
{
	if (!ensure(barrel)) { return; }

	FVector OutLaunchVelocity;
	FVector StartLocation = barrel->GetSocketLocation(FName("Projectile"));
	bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity
	(
		this,
		OutLaunchVelocity,
		StartLocation,
		HitLocation,
		LaunchSpeed,
		false,
		0,
		0,
		ESuggestProjVelocityTraceOption::DoNotTrace
	);
	if (bHaveAimSolution)
	{
		AimDirection = OutLaunchVelocity.GetSafeNormal();
		MoveBarrelTowards();
	}
}

void UTankAimingComponent::Fire()
{
	if (!ensure(barrel)) { return; }

	if (firingState == EFiringStatus::Locked || firingState == EFiringStatus::Aiming)
	{
		bool isReloaded = (FPlatformTime::Seconds() - lastFireTime) > reloadTimeInSeconds;

		if (isReloaded)
		{
			// Spawn projectile at the socket location of barrel
			auto projectile = GetWorld()->SpawnActor<AProjectile>(
				projectileBP,
				barrel->GetSocketLocation(FName("Projectile")),
				barrel->GetSocketRotation(FName("Projectile"))
				);

			projectile->LaunchProjectile(LaunchSpeed);

			lastFireTime = FPlatformTime::Seconds();

			ammoCount--;
		}
	}
}

 
int UTankAimingComponent::GetAmmoLeft() const
{
	return ammoCount;
}

void UTankAimingComponent::MoveBarrelTowards()
{
	if (!ensure(barrel && turret)) { return; }

	// Work-out difference between current barrel roation, and AimDirection
	auto BarrelRotator = barrel->GetForwardVector().Rotation();
	auto AimAsRotator = AimDirection.Rotation();
	// Change in elevation
	auto DeltaRotator = AimAsRotator - BarrelRotator;
	auto yawRotation = FMath::Abs(DeltaRotator.Yaw);

	barrel->Elevate(DeltaRotator.Pitch);

	// Always yaw the shortest way.
	if (FMath::Abs(DeltaRotator.Yaw) < 180)
	{
		turret->Rotate(DeltaRotator.Yaw);
	}
	else // Avoid going the long-way
	{
		turret->Rotate(-DeltaRotator.Yaw);
	}
}


bool UTankAimingComponent::IsBarrelMoving()
{
	if (!ensure(barrel)) { return false; }

	return barrel->GetForwardVector().Equals(AimDirection, 0.01f);
}

void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();

	// So that first fire is after initial reload
	lastFireTime = FPlatformTime::Seconds();
}

void UTankAimingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (ammoCount <= 0)
	{
		firingState = EFiringStatus::OutOfAmmo;
	}
	else if ((GetWorld()->GetTimeSeconds() - lastFireTime) > reloadTimeInSeconds)
	{
		firingState = EFiringStatus::Reloading;
	}
	else if (IsBarrelMoving())
	{
		firingState = EFiringStatus::Locked;
	}
	else
	{
		firingState = EFiringStatus::Aiming;
	}
}
