// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Logger.generated.h"

UCLASS()
class VRLOCOMOTION_API ALogger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALogger();

	// Called when actor is destroyed, or the game ends
	~ALogger();

	// Name of the file
	UPROPERTY(EditAnywhere)
	FString FileName;

	UPROPERTY(EditAnywhere)
	bool AllowOverwriting;

	UFUNCTION(BlueprintCallable, Category = "Log")
	void LogCollision(float CollisionTime);

	UFUNCTION(BlueprintCallable, Category = "Log")
	void LogTime(float StartTime, float EndTime, int Collisionss);

	UFUNCTION(BlueprintCallable, Category = "Log")
	void LogMode(int Mode, float Time);

private:
	
	void SaveData() const;
	void WriteData(FString SaveData, FString Directory, FString Appendix) const;

	TArray<float> Collisions;
	TArray<int> ModesMode;
	TArray<float> ModesTime;
	TArray<float> PathStartTime;
	TArray<float> PathEndTime;
	TArray<int> PathCollisionss;
};
