// Fill out your copyright notice in the Description page of Project Settings.

#include "VRLocomotion.h"
#include "TransformationFunctions.h"


void UTransformationFunctions::AddWorldRotationAroundPivot(USceneComponent* const Target, const USceneComponent* const Pivot, FVector DeltaRotation, const bool Sweep, FHitResult& SweepHitResult, const bool Teleport)
{
	// Calculate the relative location of the pivot from the target
	const FVector PivotRLocation = (Pivot->GetComponentLocation() - Target->GetComponentLocation());

	// Calculate the Delta Location
	const FVector DeltaLocation = PivotRLocation - PivotRLocation.RotateAngleAxis(DeltaRotation.Size(), FVector(-DeltaRotation.X, -DeltaRotation.Y, DeltaRotation.Z).GetSafeNormal());

	// Add the transform
	const FTransform DeltaTransform = FTransform(FRotator(DeltaRotation.Y, DeltaRotation.Z, DeltaRotation.X), DeltaLocation, FVector::ZeroVector);
	Target->AddWorldTransform(DeltaTransform, Sweep, (Sweep ? &SweepHitResult : nullptr), static_cast<ETeleportType>(Teleport));
}
