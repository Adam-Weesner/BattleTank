// Fill out your copyright notice in the Description page of Project Settings.

#include "TankMovementComponent.h"
#include "TankTrack.h"

void UTankMovementComponent::Initialize(UTankTrack* leftTrackToSet, UTankTrack* rightTrackToSet)
{
	leftTrack = leftTrackToSet;
	rightTrack = rightTrackToSet;
}

void UTankMovementComponent::IntendMoveForward(float Throw)
{
	if (!ensure(leftTrack && rightTrack)) { return; }

	leftTrack->SetThrottle(Throw);
	rightTrack->SetThrottle(Throw);
}

void UTankMovementComponent::IntendMoveRight(float Throw)
{
	if (!ensure(leftTrack && rightTrack)) { return; }

	leftTrack->SetThrottle(Throw);
	rightTrack->SetThrottle(-Throw);
}

void UTankMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
	auto tankForward = GetOwner()->GetActorForwardVector().GetSafeNormal();
	auto AIForwardIntention = MoveVelocity.GetSafeNormal();

	float resultingForwardThrow = FVector::DotProduct(tankForward, AIForwardIntention);

	FVector crossVector = FVector::CrossProduct(tankForward, AIForwardIntention);
	float resultingRightThrow = crossVector.Z;

	IntendMoveForward(resultingForwardThrow);
	IntendMoveRight(resultingRightThrow);
}
