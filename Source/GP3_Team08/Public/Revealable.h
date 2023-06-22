// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Revealable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class URevealable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GP3_TEAM08_API IRevealable
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, Category = "Revealable")
	void OnHide();
	UFUNCTION(BlueprintNativeEvent, Category = "Revealable")
	void OnShow();
};
