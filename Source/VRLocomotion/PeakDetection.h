// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "PeakDetection.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FComponentCustomStartSignature);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VRLOCOMOTION_API UPeakDetection : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPeakDetection();

	UPROPERTY(EditDefaultsOnly)
	float Delta;

	UPROPERTY(EditDefaultsOnly)
	float Alpha;

	UPROPERTY(EditDefaultsOnly)
	float ChecksPerSecond;

	UPROPERTY(EditDefaultsOnly)
	float MinFrequency;

	UPROPERTY(EditDefaultsOnly)
	FString RelativeComponentName;

	UFUNCTION(BlueprintPure, Category="PeakDetection")
	FVector GetMaxX();

	UFUNCTION(BlueprintPure, Category = "PeakDetection")
	FVector GetMinX();

	UFUNCTION(BlueprintPure, Category = "PeakDetection")
	FVector GetMaxY();

	UFUNCTION(BlueprintPure, Category = "PeakDetection")
	FVector GetMinY();

	UFUNCTION(BlueprintPure, Category = "PeakDetection")
	FVector GetMaxZ();

	UFUNCTION(BlueprintPure, Category = "PeakDetection")
	FVector GetMinZ();

	UFUNCTION(BlueprintPure, Category = "PeakDetection")
	float GetTimeBetweenPeaksX();

	UFUNCTION(BlueprintPure, Category = "PeakDetection")
	float GetTimeBetweenPeaksY();

	UFUNCTION(BlueprintPure, Category = "PeakDetection")
	float GetTimeBetweenPeaksZ();

	UFUNCTION(BlueprintPure, Category = "PeakDetection")
	float GetDistanceBetweenPeaksX();

	UFUNCTION(BlueprintPure, Category = "PeakDetection")
	float GetDistanceBetweenPeaksY();

	UFUNCTION(BlueprintPure, Category = "PeakDetection")
	float GetDistanceBetweenPeaksZ();

	UFUNCTION(BlueprintPure, Category = "PeakDetection")
	FVector GetDerivative();

	UFUNCTION(BlueprintPure, Category = "PeakDetection")
	float GetXYDistanceBetweenPeaksZ();

	UPROPERTY(BlueprintAssignable, Category = "Custom")
	FComponentCustomStartSignature OnPeakZ;

	UPROPERTY(BlueprintAssignable, Category = "Custom")
	FComponentCustomStartSignature OnPeakX;

	UPROPERTY(BlueprintAssignable, Category = "Custom")
	FComponentCustomStartSignature OnPeakY;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	FVector FirstDerivative;
	FVector SecondDerivative;
	FVector PreviousDerivative;
	FVector PreviousSamples;
	FVector CurrentMarker;
	FVector LastPosition;
	float LastDist;

	int Position;

	TArray<FVector> MaxX;
	TArray<FVector> MinX;

	TArray<FVector> MaxY;
	TArray<FVector> MinY;

	TArray<FVector> MaxZ;
	TArray<FVector> MinZ;

	FTimerHandle UpdateHandle;

	void Update();

	void DetectPeak(FVector Samples);

	USceneComponent* RelativeComponent;
	
};
