// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "LocSaver.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VRLOCOMOTION_API ULocSaver : public UActorComponent
{
	GENERATED_BODY()

public:
	// Name of the file
	UPROPERTY(EditAnywhere)
		FString FileName;

	// Determines whether this component records or playbacks a file
	UPROPERTY(EditAnywhere)
		bool Record;

	// How many times the position is saved per second
	UPROPERTY(EditAnywhere, meta = (ClampMin = "1", ClampMax = "120", EditCondition = "Record"))
		int RecordRate;

	// Allows the script to overwrite files
	UPROPERTY(EditAnywhere, meta = (EditCondition = "Record"))
		bool AllowOverwriting;

	// Scales the playback rate
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.1", ClampMax = "2.0", EditCondition = "!Record"))
		float PlaybackRateScale;

	// Offsets the position from the recorded data
	UPROPERTY(EditAnywhere, meta = (EditCondition = "!Record"))
		FVector PositionOffset;

	// The playback rate that is loaded from the file
	int PlaybackRate;

	// Location array storing the location information
	TArray<FVector> Locations;

	// The timer handle for how often to record/set locations
	FTimerHandle LocationSaverHandle;

	// Determines the current index in the locations array when playing back
	int CurrentTimeIndex;

	// Writes the location data to a file
	void WriteToFile();

	// Reads the location data from a file
	void ReadFromFile();

	// Adds the current location to the Locations array
	void RecordCurrentLocation();

	// Sets the actors current location based on the Locations array
	void SetCurrentLocation();

public:
	// Sets default values for this component's properties
	ULocSaver();

	// Called when actor is destroyed, or the game ends
	~ULocSaver();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;



};
