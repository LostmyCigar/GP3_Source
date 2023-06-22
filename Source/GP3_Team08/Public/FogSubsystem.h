// By Amos Johan Persson

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "FogSubsystem.generated.h"

/**
 * Class for managing memory slots in material parameter collection used for fog. All requests for write must go through this class.
 * FString object names -> FName parameter slots are stored in a map, together with a timestamp for each FString in another map.
 * Available slots are kept in a queue where unused slots can be recycled. 
 * 
 * It's possible to reserve a name so it is never deallocated with the ReserveName function
 */
UCLASS()
class GP3_TEAM08_API UFogSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UFogSubsystem();

	// Begin USubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End USubsystem

private:
	TMap<FString, FName> AllocationsByName;
	TMap<FString, float> TimestampsByName;
	TQueue<FName> FreeSlots;
	TArray<FString> ReservedNames;

private:
	void WriteRadius(FName Address, float Radius);
	void WriteAlpha(FName Address, float Alpha);
	void WriteData(FName Address, FVector Location, float Alpha);

	void ClearDataSlot(FString Actor);
	void Allocate(FString Actor, FName Slot);
	void Timestamp(FString Actor);
	FName RequestDataSlot();

public:
	UPROPERTY(EditDefaultsOnly)
	UMaterialParameterCollection* FogData;

public:

	UFUNCTION(BlueprintCallable)
	bool UpdateFogInfo(AActor* Source, FVector Location, float Alpha, float Radius);

	UFUNCTION(BlueprintCallable)
	bool Register(AActor* Source);

	UFUNCTION(BlueprintCallable)
	void Deregister(AActor* Source);

	UFUNCTION(BlueprintCallable)
	bool ReserveName(AActor* Source);


};