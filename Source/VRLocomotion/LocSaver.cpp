// Fill out your copyright notice in the Description page of Project Settings.

#include "VRLocomotion.h"
#include "LocSaver.h"


// Sets default values for this component's properties
ULocSaver::ULocSaver()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// Default values
	RecordRate = 90;
	Record = true;
	AllowOverwriting = false;
	CurrentTimeIndex = 0;
	PlaybackRateScale = 1.0f;
	PlaybackRate = 90;
	PositionOffset = FVector(0.0, 0.0, 0.0);
	RotationOffset = FVector(0.0, 0.0, 0.0);
	ApplyRotation = true;
	Velocity = FVector(0.0, 0.0, 0.0);
	VelocityInfluencedByPlayback = false;
	RecalculateVelocity = false;
}

// Called when actor is destroyed, or the game ends
ULocSaver::~ULocSaver()
{
	// Write the location data if we are recording
	if (Record)
	{
		WriteToFile();
	}
}


// Called when the game starts
void ULocSaver::BeginPlay()
{
	Super::BeginPlay();

	// Check if we should record or playback a file
	if (Record)
	{
		// Sets a timer for RecordCurrentLocation at the specified record rate
		GetOwner()->GetWorldTimerManager().SetTimer(LocationSaverHandle, this, &ULocSaver::RecordCurrentLocation, 1 / float(RecordRate), true);
	}
	else
	{
		ReadFromFile();

		// Convert roation offset from euler to quaternion
		RotationOffsetQuat = RotationOffset.Rotation().Quaternion();

		// Sets a timer for SetCurrentLocation at the specified playbackrate
		GetOwner()->GetWorldTimerManager().SetTimer(LocationSaverHandle, this, &ULocSaver::SetCurrentLocation, 1 / (PlaybackRate * PlaybackRateScale), true);
	}
}


// Called every frame - not used in this script
void ULocSaver::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


// Writes the location data to a file
void ULocSaver::WriteToFile() const
{
	// Check if file name is not null
	if (!FileName.Equals("") && Locations.Num() > 0)
	{
		const FString Directory = FPaths::GameDir() + "LocationData/";
		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

		// Verify or create the directory
		if (!PlatformFile.DirectoryExists(*Directory))
		{
			PlatformFile.CreateDirectory(*Directory);

			// Failed to create the directory
			if (!PlatformFile.DirectoryExists(*Directory))
			{
				return;
			}
		}

		// Convert array storing location data to a string
		FString SaveData = FString::FromInt(RecordRate) + "\n";
		for (int32 i = 0; i != Locations.Num(); ++i)
		{
			SaveData.Append(Locations[i].ToString() + "\n");
			SaveData.Append(Rotations[i].ToString() + "\n");
			SaveData.Append(VelocityArray[i].ToString() + "\n");
		}

		// Save the string to file
		FString AbsoluteFilePath = Directory + FileName + ".loc";
		if (PlatformFile.CreateDirectoryTree(*Directory))
		{
			// Write ther file to specified name if we are allowed to overwrite, or the file does not already exist
			if (AllowOverwriting)
			{
				FFileHelper::SaveStringToFile(SaveData, *AbsoluteFilePath);
			}

			// Otherwise, increment the filename and try again
			else
			{
				// Max iterations of loop
				const int MaxFileIndex = 100;

				// Check for non existing file names with increments up to specified max
				int i = -1;
				do
				{
					i++;
					AbsoluteFilePath = Directory + FileName + FString::FromInt(i) + ".loc";
				} while (PlatformFile.FileExists(*AbsoluteFilePath) && i < MaxFileIndex);

				// Save the data
				if (!PlatformFile.FileExists(*AbsoluteFilePath))
				{
					FFileHelper::SaveStringToFile(SaveData, *AbsoluteFilePath);
				}
			}
		}
	}
}


// Reads the location data from a file
void ULocSaver::ReadFromFile()
{
	// Check if file name is not null
	if (!FileName.Equals(""))
	{
		const FString Directory = FPaths::GameDir() + "LocationData/";
		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

		// Verify the directory
		if (!PlatformFile.DirectoryExists(*Directory))
		{
			// Print error message
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red, TEXT("LocSaver Error: Directory doesn't exist"));
			}
			return;
		}

		// Load data to a string
		FString LoadData;
		const FString AbsoluteFilePath = Directory + FileName + ".loc";
		if (PlatformFile.CreateDirectoryTree(*Directory))
		{
			if (PlatformFile.FileExists(*AbsoluteFilePath))
			{
				FFileHelper::LoadFileToString(LoadData, *AbsoluteFilePath);
			}
			else
			{
				return;
			}
		}

		// Use this to split the string by certain characters
		const TCHAR * Delims[] = { TEXT("X"), TEXT("Y"), TEXT("Z"), TEXT(" "), TEXT("\n"), TEXT("="), TEXT("W") };

		// Split string into an array of strings
		TArray<FString> LoadDataArray;
		LoadData.ParseIntoArray(LoadDataArray, Delims, 7, true);

		// Convert array of strings to array of FVectors/FQuats
		PlaybackRate = FCString::Atoi(*LoadDataArray[0]);
		for (int i = 1; i < LoadDataArray.Num(); i = i + 10)
		{
			Locations.Add(FVector(FCString::Atof(*LoadDataArray[i]), FCString::Atof(*LoadDataArray[i + 1]), FCString::Atof(*LoadDataArray[i + 2])));
			Rotations.Add(FQuat(FCString::Atof(*LoadDataArray[i + 3]), FCString::Atof(*LoadDataArray[i + 4]), FCString::Atof(*LoadDataArray[i + 5]), FCString::Atof(*LoadDataArray[i + 6])));
			VelocityArray.Add(FVector(FCString::Atof(*LoadDataArray[i + 7]), FCString::Atof(*LoadDataArray[i + 8]), FCString::Atof(*LoadDataArray[i + 9])));
		}
	}
}


// Adds the current location to the Locations array
void ULocSaver::RecordCurrentLocation()
{
	Locations.Add(GetOwner()->GetTransform().GetTranslation());
	Rotations.Add(GetOwner()->GetTransform().GetRotation());
	Velocity = GetOwner()->GetVelocity();
	VelocityArray.Add(Velocity);
}


// Sets the actors current location based on the Locations array
void ULocSaver::SetCurrentLocation()
{
	// If the script has iterated over the last index of the array, stop the timer and exit
	if (CurrentTimeIndex + 1 >= Locations.Num())
	{
		GetOwner()->GetWorldTimerManager().ClearTimer(LocationSaverHandle);
		Velocity = FVector(0, 0, 0);
		return;
	}

	// Update the velocity variable
	SetVelocity();

	// Updates the location - PositionOffset is added to the location from the file
	GetOwner()->SetActorLocation(Locations[CurrentTimeIndex] + PositionOffset, false);

	if (ApplyRotation)
	{
		// Updates the rotation - RotationOffsetQuat is multiplied with the roation value as that is how you add quaternions
		GetOwner()->SetActorRotation(RotationOffsetQuat * Rotations[CurrentTimeIndex]);
	}

	CurrentTimeIndex++;
}


/**
* Get the current velocity of the object
* @return The current velocity
*/
FVector ULocSaver::GetVelocity() const
{
	return Velocity;
}


// Sets the velocity variable - FORCEINLINE to improve performance
FORCEINLINE void ULocSaver::SetVelocity()
{
	bool NewVelocity = false;

	if (!RecalculateVelocity)
	{
		// Read the velocity from the array based on the file
		Velocity = VelocityArray[CurrentTimeIndex];
		NewVelocity = true;
	}
	else
	{
		// Calculate a velocity based on the location movement
		FVector TempVelocity = (Locations[CurrentTimeIndex + 1] - Locations[CurrentTimeIndex]) / (1 / (PlaybackRate * PlaybackRateScale));
		if (TempVelocity.SizeSquared() != 0)
		{
			Velocity = TempVelocity;
			NewVelocity = true;
		}
	}

	// Scale the velocity if a new velocity has been calculated, and the boolean has been checked in the editor
	if (NewVelocity && VelocityInfluencedByPlayback)
	{
		Velocity *= PlaybackRateScale;
	}
}