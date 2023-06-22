// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Activatable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UActivatable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GP3_TEAM08_API IActivatable
{
	GENERATED_BODY()


public:

	UFUNCTION(BlueprintNativeEvent)
	void Activate();
};
