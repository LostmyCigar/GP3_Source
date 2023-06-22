// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Prism.h"
#include "LanternHolder.h"
#include "Lantern.h"
#include "StationaryPrism.generated.h"

/**
 * 
 */
UCLASS()
class GP3_TEAM08_API AStationaryPrism : public APrism, public ILanternHolder
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable)
	void GiveLantern(AActor* interactor);

	UFUNCTION(BlueprintCallable)
	void HoldLantern(AActor* interactor, USceneComponent* parent);

	UFUNCTION(BlueprintCallable)
	void ReceiveLantern(ALantern* LanternToHold) override;

	UFUNCTION(BlueprintCallable)
	void RemoveLantern() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName LanternSocketName;

public:
	UPROPERTY(BlueprintReadWrite)
	bool IsHoldingLantern;

private:
	ALantern* Lantern;
};
