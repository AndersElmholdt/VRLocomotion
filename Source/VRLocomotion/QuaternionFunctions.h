// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "QuaternionFunctions.generated.h"

/**
 * 
 */
UCLASS()
class VRLOCOMOTION_API UQuaternionFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	/**
	 * Converts a rotation to its corresponding quaternion representation.
	 * @param Rotation		The rotation that should be converted.
	 * @return				The resulting quaternion from the conversion.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Math|Quaternion", meta = (DisplayName = "Convert To Quaternion"))
	static FQuat ConvertToQuaternion(const FRotator& Rotation);

	/**
	 * Combines two quaternion rotations (This is done by matrix multiplication, so orders matter).
	 * @param FirstQuat		The quaternion of the first rotation.
	 * @param SecondQuat	The quaternion of the second rotation.
	 * @return				The combined rotation as a quaternion.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Math|Quaternion", meta = (DisplayName = "Combine Quaternions"))
	static FQuat CombineQuaternions(const FQuat& FirstQuat, const FQuat& SecondQuat);

	/**
	 * Converts a quaternion to its corresponding rotation value.
	 * @param Quaternion	The quaternion that should be converted.
	 * @return				The resulting FRotator struct from the conversion.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Math|Quaternion", meta = (DisplayName = "Convert To Rotator"))
	static FRotator ConvertToRotator(const FQuat& Quaternion);

	/**
	* Inverts a quaternion.
	* @param Quaternion		The quaternion that should be inverted.
	* @return				The inverted quaternion.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Math|Quaternion", meta = (DisplayName = "Invert Quaternion"))
	static FQuat InvertQuaternion(const FQuat& Quaternion);
	
};
