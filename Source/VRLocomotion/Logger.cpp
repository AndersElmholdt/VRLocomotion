// Fill out your copyright notice in the Description page of Project Settings.

#include "VRLocomotion.h"
#include "Logger.h"

// Sets default values
ALogger::ALogger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

ALogger::~ALogger()
{
	SaveData();
}

void ALogger::LogCollision(float CollisionTime) {
	Collisions.Add(CollisionTime);
}

void ALogger::LogTime(float StartTime, float EndTime, int Collisionss) {
	PathStartTime.Add(StartTime);
	PathEndTime.Add(EndTime);
	PathCollisionss.Add(Collisionss);
}

void ALogger::LogMode(int Mode, float Time) {
	ModesMode.Add(Mode);
	ModesTime.Add(Time);
}

void ALogger::SaveData() const {
	// Check if file name is not null
	if (!FileName.Equals(""))
	{
		const FString Directory = FPaths::GameDir() + "TestData/";
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

		// Convert array storing collision data to a string
		FString SaveData = "";
		for (int32 i = 0; i != Collisions.Num(); ++i)
		{
			SaveData.Append(FString::SanitizeFloat(Collisions[i]) + "\n");
		}
		WriteData(SaveData, Directory, FString(TEXT("Collision")));

		SaveData = "";
		for (int32 i = 0; i != ModesMode.Num(); ++i)
		{
			SaveData.Append(FString::FromInt(ModesMode[i]) + " " + FString::SanitizeFloat(ModesTime[i]) + "\n");
		}
		WriteData(SaveData, Directory, FString(TEXT("Modes")));

		SaveData = "";
		for (int32 i = 0; i != PathStartTime.Num(); ++i)
		{
			SaveData.Append(FString::SanitizeFloat(PathStartTime[i]) + " " + FString::SanitizeFloat(PathEndTime[i]) + " " + FString::FromInt(PathCollisionss[i]) +  "\n");
		}
		WriteData(SaveData, Directory, FString(TEXT("Paths")));
	}
}

void ALogger::WriteData(FString SaveData, FString Directory, FString Appendix) const {
	// Save the string to file
	FString AbsoluteFilePath = Directory + Appendix + FileName + ".log";
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
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
				AbsoluteFilePath = Directory + Appendix + FileName + FString::FromInt(i) + ".log";
			} while (PlatformFile.FileExists(*AbsoluteFilePath) && i < MaxFileIndex);

			// Save the data
			if (!PlatformFile.FileExists(*AbsoluteFilePath))
			{
				FFileHelper::SaveStringToFile(SaveData, *AbsoluteFilePath);
			}
		}
	}
}
