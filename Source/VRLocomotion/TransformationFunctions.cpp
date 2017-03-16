// Fill out your copyright notice in the Description page of Project Settings.

#include "VRLocomotion.h"
#include "TransformationFunctions.h"


void UTransformationFunctions::AddWorldRotationAroundPivot(USceneComponent* const Target, const FVector Pivot, FVector DeltaRotation, const bool Sweep, FHitResult& SweepHitResult, const bool Teleport)
{
	// Calculate the relative location of the pivot from the target
	const FVector PivotRLocation = (Pivot - Target->GetComponentLocation());

	// Calculate the Delta Location
	const FVector DeltaLocation = PivotRLocation - PivotRLocation.RotateAngleAxis(DeltaRotation.Size(), FVector(-DeltaRotation.X, -DeltaRotation.Y, DeltaRotation.Z).GetSafeNormal());

	// Add the transform
	const FTransform DeltaTransform = FTransform(FRotator(DeltaRotation.Y, DeltaRotation.Z, DeltaRotation.X), DeltaLocation, FVector::ZeroVector);
	Target->AddWorldTransform(DeltaTransform, Sweep, (Sweep ? &SweepHitResult : nullptr), static_cast<ETeleportType>(Teleport));
}


float ApplyThreshold(const float Input, const float Threshold)
{
	const float Difference = ((Input < 0) ? -1 : 1) * Input - Threshold;
	return (Difference > 0) ? Difference : 0;
}
