// By Amos Johan Persson

#include "FogSubsystem.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialParameterCollection.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMaterialLibrary.h"

UFogSubsystem::UFogSubsystem()
{
	static ConstructorHelpers::FObjectFinder<UMaterialParameterCollection> DataCollection(TEXT("/Game/Materials/Fog/FirePositions"));

	FogData = DataCollection.Object;

	if (IsValid(FogData))
	{
		TArray<FName> ParameterNames = TArray<FName>();
		FogData->GetParameterNames(ParameterNames, true);

		for (FName name : ParameterNames)
		{
			FreeSlots.Enqueue(name);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Bad Fog Collection"));
	}
}

void UFogSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	
}

void UFogSubsystem::Deinitialize()
{

}

void UFogSubsystem::WriteRadius(FName Address, float Radius)
{
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), FogData, Address, Radius);
}

void UFogSubsystem::WriteAlpha(FName Address, float Alpha)
{
	FVector4 Current = UKismetMaterialLibrary::GetVectorParameterValue(GetWorld(), FogData, Address);
	const FLinearColor Values = FLinearColor(Current.X, Current.Y, Current.Z, Alpha);
	UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), FogData, Address, Values);
}

void UFogSubsystem::WriteData(FName Address, FVector Location, float Alpha)
{
	const FLinearColor Values = FLinearColor(Location.X, Location.Y, Location.Z, Alpha);
	UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), FogData, Address, Values);
}


void UFogSubsystem::ClearDataSlot(FString Actor)
{
	if (!AllocationsByName.Contains(Actor))
	{
		if (TimestampsByName.Contains(Actor))
		{
			TimestampsByName.Remove(Actor);
		}
		return;
	}

	FName Address = AllocationsByName[Actor];
	AllocationsByName.Remove(Actor);
	TimestampsByName.Remove(Actor);
	WriteAlpha(Address, 0.0);
	FreeSlots.Enqueue(Address);
}

void UFogSubsystem::Allocate(FString Actor, FName Slot)
{
	AllocationsByName.Add(Actor, Slot);
}

void UFogSubsystem::Timestamp(FString Actor)
{
	TimestampsByName.Add(Actor, UGameplayStatics::GetRealTimeSeconds(GetWorld()));
}

FName UFogSubsystem::RequestDataSlot()
{
	FName Address;
	bool SlotFound = FreeSlots.Dequeue(Address);

	if (!SlotFound)
	{
		// No free slots -> recycle oldest
		TPair<FString, float> bestPair = TPair<FString, float>(FString(), INFINITY);

		for (TPair<FString, float> keyVal : TimestampsByName)
		{
			// Sanity check only, reserved names should not get timestamped
			if (ReservedNames.Contains(keyVal.Key))
			{
				continue;
			}

			if (keyVal.Value < bestPair.Value)
				bestPair = keyVal;
		}

		if (!AllocationsByName.Contains(bestPair.Key)) 
		{
			return Address;
		}

		Address = AllocationsByName[bestPair.Key];
		ClearDataSlot(bestPair.Key);
	}

	return Address;
}

bool UFogSubsystem::UpdateFogInfo(AActor* Source, FVector Location, float Alpha, float Radius)
{
	if (!IsValid(Source)) return false;
	if (!AllocationsByName.Contains(Source->GetName())) return false;

	FName Address = AllocationsByName[Source->GetName()];

	if (!Address.IsNone())
	{
		WriteRadius(Address, Radius);
		WriteData(Address, Location, Alpha);

		return true;
	}
	else
	{
		return false;
	}

}

bool UFogSubsystem::Register(AActor* Source)
{
	if (!IsValid(Source)) return false;

	FString SourceName = Source->GetName();

	if (!AllocationsByName.Contains(SourceName))
	{
		FName Address = RequestDataSlot();

		if (!Address.IsNone())
		{
			Allocate(SourceName, Address);
			Timestamp(SourceName);
			return true;
		}
	}

	return false;
}

void UFogSubsystem::Deregister(AActor* Source)
{
	if (!IsValid(Source)) return;

	FString SourceName = Source->GetName();

	if (AllocationsByName.Contains(SourceName))
	{
		ClearDataSlot(SourceName);
	}
}

bool UFogSubsystem::ReserveName(AActor* Source)
{
	FString SourceName = Source->GetName();
	FName Address = RequestDataSlot();

	if (Address.IsNone())
		return false;

	ReservedNames.Add(SourceName);
	Allocate(SourceName, Address);
	// DO NOT TIMESTAMP
	return true;
}