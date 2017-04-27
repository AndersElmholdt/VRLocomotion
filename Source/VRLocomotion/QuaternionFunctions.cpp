// Fill out your copyright notice in the Description page of Project Settings.

#include "VRLocomotion.h"
#include "QuaternionFunctions.h"


FQuat UQuaternionFunctions::ConvertToQuaternion(const FRotator& Rotation)
{
	return Rotation.Quaternion();
}

FQuat UQuaternionFunctions::CombineQuaternions(const FQuat& FirstQuat, const FQuat& SecondQuat)
{
	return FirstQuat * SecondQuat;
}

FRotator UQuaternionFunctions::ConvertToRotator(const FQuat& Quaternion)
{
	return Quaternion.Rotator();
}

FQuat UQuaternionFunctions::InvertQuaternion(const FQuat& Quaternion)
{
	return Quaternion.Inverse();
}
