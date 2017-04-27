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

float UTransformationFunctions::ApplyThreshold(const float Input, const float Threshold)
{
	const float UpperBounds = 1.0f - Threshold;
	const float ThresholdedValue = FMath::Clamp((1.0f / UpperBounds) * FMath::Abs(Input) - Threshold, 0.0f, 1.0f);
	return ThresholdedValue * FMath::Sign(Input);
}

void UTransformationFunctions::AddLocalRotationQuat(USceneComponent* Target, const FQuat& DeltaRotation, const bool Sweep, FHitResult& SweepHitResult, const bool Teleport){
	Target->AddLocalRotation(DeltaRotation, Sweep, (Sweep ? &SweepHitResult : nullptr), static_cast<ETeleportType>(Teleport));
}

void UTransformationFunctions::AddLocalRotationQuats(USceneComponent* Target, const FRotator& DeltaRotation, const bool Sweep, FHitResult& SweepHitResult, const bool Teleport) {
	AddLocalRotationQuat(Target, DeltaRotation.Quaternion(), Sweep, SweepHitResult, Teleport);
}
