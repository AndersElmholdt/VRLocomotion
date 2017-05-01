// Fill out your copyright notice in the Description page of Project Settings.

#include "VRLocomotion.h"
#include "PeakDetection.h"
#include "PeakReceiver.h"


// Sets default values for this component's properties
UPeakDetection::UPeakDetection()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	Delta = 1.0f;
	Alpha = 0.1f;
	ChecksPerSecond = 20.0f;
	MinFrequency = 3.5f;
	LastPosition = FVector::ZeroVector;
}

// Called when the game starts
void UPeakDetection::BeginPlay()
{
	Super::BeginPlay();

	MaxX.Add(FVector::ZeroVector);
	MinX.Add(FVector::ZeroVector);
	MaxY.Add(FVector::ZeroVector);
	MinY.Add(FVector::ZeroVector);
	MaxZ.Add(FVector::ZeroVector);
	MinZ.Add(FVector::ZeroVector);

	// Looks for a component that is also attached to the same actor as this, with the name given by RelativeComponentName.
	TArray<USceneComponent*> Components;
	GetAttachmentRoot()->GetChildrenComponents(true, Components);
	for (USceneComponent* Component : Components)
	{
		if (Component->GetName().Equals(RelativeComponentName))
		{
			RelativeComponent = Component;
			break;
		}
	}

	GetWorld()->GetTimerManager().SetTimer(UpdateHandle, this, &UPeakDetection::Update, 1/ChecksPerSecond, true);
}

void UPeakDetection::Update()
{
	FVector Samples = this->GetComponentLocation();
	
	// Calculate a relative location compared to the RelativeComponent
	if (RelativeComponent)
	{
		Samples -= RelativeComponent->GetComponentLocation();

		// Counterrotate by parents rotation
		//Samples.RotateAngleAxis(-RelativeComponent->GetComponentRotation().Yaw, FVector::UpVector);
		Samples.RotateAngleAxis(GetComponentRotation().Yaw, FVector::UpVector);
		//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, FString::SanitizeFloat(GetComponentRotation().Yaw));
	}
	
	DetectPeak(Samples);
	Position++;
}

void UPeakDetection::DetectPeak(FVector Samples)
{
	if (PreviousSamples != Samples)
	{
		// LOW PASS FILTERING OF MARKER DATA:
		Samples.X = Alpha*Samples.X + (1 - Alpha) * PreviousSamples.X;
		Samples.Y = Alpha*Samples.Y + (1 - Alpha) * PreviousSamples.Y;
		Samples.Z = Alpha*Samples.Z + (1 - Alpha) * PreviousSamples.Z;

		// The first derivatives of the marker data (sample) and previous marker data (previousSample) are calculated.
		FirstDerivative = (Samples - PreviousSamples);
		SecondDerivative = (FirstDerivative - PreviousDerivative);

		// X-COMPONENT PEAK DETECTION:
		if (PreviousDerivative.X >= 0 && FirstDerivative.X < 0 && FMath::Abs(SecondDerivative.X) > Delta)
		{
			// Minimum found
			MinX.Add(FVector(Position, Samples.X, GetWorld()->GetTimeSeconds()));
			OnPeakX.Broadcast();
		}
		else if (PreviousDerivative.X <= 0 && FirstDerivative.X > 0 && FMath::Abs(SecondDerivative.X) > Delta)
		{
			// Maximum found
			MaxX.Add(FVector(Position, Samples.X, GetWorld()->GetTimeSeconds()));
		}

		// Y-COMPONENT PEAK DETECTION:
		if (PreviousDerivative.Y >= 0 && FirstDerivative.Y < 0 && FMath::Abs(SecondDerivative.Y) > Delta)
		{
			// Minimum found
			MinY.Add(FVector(Position, Samples.Y, GetWorld()->GetTimeSeconds()));
			OnPeakY.Broadcast();
		}
		else if (PreviousDerivative.Y <= 0 && FirstDerivative.Y > 0 && FMath::Abs(SecondDerivative.Y) > Delta)
		{
			// Maximum found
			MaxY.Add(FVector(Position, Samples.Y, GetWorld()->GetTimeSeconds()));
		}

		// Z-COMPONENT PEAK DETECTION:
		if (PreviousDerivative.Z >= 0 && FirstDerivative.Z < 0 && FMath::Abs(SecondDerivative.Z) > Delta && (GetWorld()->GetTimeSeconds() - MinZ.Last().Z) > 1 / MinFrequency)
		{
			// Minimum found
			LastDist = FVector::Dist(FVector(Samples.X, Samples.Y, 0), FVector(LastPosition.X, LastPosition.Y, 0));
			LastPosition = Samples;
			MinZ.Add(FVector(Position, Samples.Z, GetWorld()->GetTimeSeconds()));
			OnPeakZ.Broadcast();
		}
		else if (PreviousDerivative.Z <= 0 && FirstDerivative.Z > 0 && FMath::Abs(SecondDerivative.Z) > Delta && (GetWorld()->GetTimeSeconds() - MaxZ.Last().Z) > 1 / MinFrequency)
		{
			// Maximum found
			MaxZ.Add(FVector(Position, Samples.Z, GetWorld()->GetTimeSeconds()));
			//OnPeakZ.Broadcast();
		}

		PreviousSamples = Samples;
		PreviousDerivative = FirstDerivative;
	}
}

FVector UPeakDetection::GetDerivative()
{
	return PreviousDerivative;
}

FVector UPeakDetection::GetMaxX()
{
	return MaxX.Last();
}

FVector UPeakDetection::GetMaxY()
{
	return MaxY.Last();
}

FVector UPeakDetection::GetMaxZ()
{
	return MaxZ.Last();
}

FVector UPeakDetection::GetMinX()
{
	return MinX.Last();
}

FVector UPeakDetection::GetMinY()
{
	return MinY.Last();
}

FVector UPeakDetection::GetMinZ()
{
	return MinZ.Last();
}

float UPeakDetection::GetTimeBetweenPeaksX()
{
	float DifferenceX = FMath::Abs(MaxX.Last().Z - MinX.Last().Z);
	return DifferenceX;
}

float UPeakDetection::GetTimeBetweenPeaksY()
{
	float DifferenceY = FMath::Abs(MaxY.Last().Z - MinY.Last().Z);
	return DifferenceY;
}

float UPeakDetection::GetTimeBetweenPeaksZ()
{
	float DifferenceZ = FMath::Abs(MaxZ.Last().Z - MinZ.Last().Z);
	return DifferenceZ;
}

float UPeakDetection::GetDistanceBetweenPeaksX()
{
	float DistanceX = FMath::Abs(MaxX.Last().Y - MinX.Last().Y);
	return DistanceX;
}

float UPeakDetection::GetDistanceBetweenPeaksY()
{
	float DistanceY = FMath::Abs(MaxY.Last().Y - MinY.Last().Y);
	return DistanceY;
}

float UPeakDetection::GetDistanceBetweenPeaksZ()
{
	float DistanceZ = FMath::Abs(MaxZ.Last().Y - MinZ.Last().Y);
	return DistanceZ;
}

float UPeakDetection::GetXYDistanceBetweenPeaksZ()
{
	return LastDist;
}
