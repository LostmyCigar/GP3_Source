// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PrismInteractable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UPrismInteractable : public UInterface
{
	GENERATED_BODY()
};


class GP3_TEAM08_API IPrismInteractable
{
	GENERATED_BODY()

public:

	/*
	Called every frame the object is hit by beam
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "Prism interaction")
	void PrismInteraction();


	/*
	Called THE frame the object is no longer hit by the beam
	*/
	UFUNCTION(BlueprintNativeEvent, Category = "Prism interaction")
	void StopPrismInteraction();

};
