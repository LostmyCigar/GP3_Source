// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lantern.h"
#include "UObject/Interface.h"
#include "LanternHolder.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, NotBlueprintable)
class ULanternHolder : public UInterface
{
	GENERATED_BODY()
};



class GP3_TEAM08_API ILanternHolder
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	virtual void ReceiveLantern(ALantern* LanternToHold);
	UFUNCTION(BlueprintCallable)
	virtual void RemoveLantern();

	FName LanternSocketName;
};
