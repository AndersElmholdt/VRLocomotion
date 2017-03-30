// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "TransformationFunctions.generated.h"

/**
 * 
 */
UCLASS()
class VRLOCOMOTION_API UTransformationFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	/**
	* Adds a delta to the rotation of the component in world space around a specified pivot.
	* @param Target			The component that should be rotated.
	* @param Pivot			The location that should be used as pivot.
	* @param DeltaRotation	Change in rotation in world space for the component.
	* @param SweepHitResult	Hit result from any impact if sweep is true.
	* @param Sweep			Whether we sweep to the destination (currently not supported for rotation).
	* @param Teleport		Whether we teleport the physics state (if physics collision is enabled for this object).
	*						If true, physics velocity for this object is unchanged (so ragdoll parts are not affected by change in location).
	*						If false, physics velocity is updated based on the change in position (affecting ragdoll parts).
	*						If CCD is on and not teleporting, this will affect objects along the entire sweep volume.
	*/
	UFUNCTION(BlueprintCallable, Category = "Utilities|Transformation", meta = (DisplayName = "AddWorldRotationAroundPivot", AdvancedDisplay = "Sweep,SweepHitResult,Teleport"))
	static void AddWorldRotationAroundPivot(USceneComponent* const Target, const FVector Pivot, const FVector DeltaRotation, const bool Sweep, FHitResult& SweepHitResult, const bool Teleport);
	
	/**
	 * Applies a threshold to the incoming number.
	 * @param Input			The input number that should have the threshold applied to it.
	 * @param Threshold		The threshold that should be applied.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Math|Float", meta = (DisplayName = "Apply Threshold"))
	static float ApplyThreshold(const float Input, const float Threshold);

	UFUNCTION(BlueprintCallable, Category = "Utilities|Transformation", meta = (DisplayName = "Hello", AdvancedDisplay = "bSweep,SweepHitResult,bTeleport"))
	static void AddLocalRotationQuat(USceneComponent* Target, const FQuat& DeltaRotation, bool Sweep, FHitResult& SweepHitResult, const bool Teleport);

	UFUNCTION(BlueprintCallable, Category = "Utilities|Transformation", meta = (DisplayName = "Hallo", AdvancedDisplay = "bSweep,SweepHitResult,bTeleport"))
	static void AddLocalRotationQuats(USceneComponent* Target, const FRotator& DeltaRotation, bool Sweep, FHitResult& SweepHitResult, const bool Teleport);
};
