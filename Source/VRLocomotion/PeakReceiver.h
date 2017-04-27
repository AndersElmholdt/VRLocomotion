// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PeakReceiver.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPeakReceiver : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

/**
 * 
 */
class VRLOCOMOTION_API IPeakReceiver
{
	GENERATED_IINTERFACE_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	void OnPeakZ(bool Right);
	
};
