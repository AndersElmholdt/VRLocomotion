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
	RecordRate = 60;
	Record = true;
	AllowOverwriting = false;
	CurrentTimeIndex = 0;
	PlaybackRateScale = 1.0f;
	PlaybackRate = 60;
	PositionOffset = FVector(0.0, 0.0, 0.0);
}

// Called when actor is destroyed, or the game ends
ULocSaver::~ULocSaver() {
	// Write the location data when game is closed
	if (Record)
		WriteToFile();
}


// Called when the game starts
void ULocSaver::BeginPlay()
{
	Super::BeginPlay();

	// If recording, call the function RecordCurrentLocation at the specified record rate
	if (Record) {
		GetOwner()->GetWorldTimerManager().SetTimer(LocationSaverHandle, this, &ULocSaver::RecordCurrentLocation, 1 / float(RecordRate), true);
	}

	// Otherwise, read the file and call SetCurrentLocation instead
	else {
		ReadFromFile();
		GetOwner()->GetWorldTimerManager().SetTimer(LocationSaverHandle, this, &ULocSaver::SetCurrentLocation, 1 / (PlaybackRate * PlaybackRateScale), true);
	}
}


// Called every frame - not used in this script
void ULocSaver::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


// Writes the location data to a file
void ULocSaver::WriteToFile() {
	// Check if file name is not null
	if (!FileName.Equals("")) {
		FString Directory = FPaths::GameDir() + "LocationData/";
		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

		// Verify or create the directory
		if (!PlatformFile.DirectoryExists(*Directory)) {
			PlatformFile.CreateDirectory(*Directory);

			// Failed to create the directory
			if (!PlatformFile.DirectoryExists(*Directory)) {
				return;
			}
		}

		// Convert array storing location data to a string
		FString SaveData = FString::FromInt(RecordRate) + "\n";
		for (auto& vec : Locations) {
			SaveData.Append(vec.ToString() + "\n");
		}

		// Save the string to file
		FString AbsoluteFilePath = Directory + FileName + ".loc";
		if (PlatformFile.CreateDirectoryTree(*Directory)) {
			if (AllowOverwriting || !PlatformFile.FileExists(*AbsoluteFilePath)) {
				FFileHelper::SaveStringToFile(SaveData, *AbsoluteFilePath);
			}
		}
	}
}


// Reads the location data from a file
void ULocSaver::ReadFromFile() {
	// Check if file name is not null
	if (!FileName.Equals("")) {
		FString Directory = FPaths::GameDir() + "LocationData/";
		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

		// Verify the directory
		if (!PlatformFile.DirectoryExists(*Directory)) {
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red, TEXT("LocSaver Error: Directory doesn't exist"));
			return;
		}

		// Load data to a string
		FString LoadData;
		FString AbsoluteFilePath = Directory + FileName + ".loc";
		if (PlatformFile.CreateDirectoryTree(*Directory)) {
			if (PlatformFile.FileExists(*AbsoluteFilePath)) {
				FFileHelper::LoadFileToString(LoadData, *AbsoluteFilePath);
			}
			else {
				return;
			}
		}

		// Use this to split the string by certain characters
		const TCHAR * Delims[] = { TEXT("X"), TEXT("Y"), TEXT("Z"), TEXT(" "), TEXT("\n"), TEXT("=") };

		// Split string into an array of strings
		TArray<FString> LoadDataArray;
		LoadData.ParseIntoArray(LoadDataArray, Delims, 6, true);

		// Convert array of strings to array of FVectors
		PlaybackRate = FCString::Atoi(*LoadDataArray[0]);
		for (int i = 1; i < LoadDataArray.Num(); i = i + 3) {
			Locations.Add(FVector(FCString::Atof(*LoadDataArray[i]), FCString::Atof(*LoadDataArray[i + 1]), FCString::Atof(*LoadDataArray[i + 2])));
		}
	}
}


// Adds the current location to the Locations array
void ULocSaver::RecordCurrentLocation() {
	Locations.Add(GetOwner()->GetTransform().GetTranslation());
}


// Sets the actors current location based on the Locations array
void ULocSaver::SetCurrentLocation() {
	// If the script has iterated over the last index of the array, stop the timer and exit
	if (CurrentTimeIndex + 1 >= Locations.Num()) {
		GetOwner()->GetWorldTimerManager().ClearTimer(LocationSaverHandle);
		return;
	}

	// Updates the location
	GetOwner()->SetActorLocation(Locations[++CurrentTimeIndex] + PositionOffset, false);
}

