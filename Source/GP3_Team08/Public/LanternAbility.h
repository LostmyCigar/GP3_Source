// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LanternAbility.generated.h"


UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GP3_TEAM08_API ULanternAbility : public UActorComponent
{
	GENERATED_BODY()

public:	
	ULanternAbility();

protected:
	virtual void BeginPlay() {};

public:	
	//Runs in tick when the ability is active on lantern
	virtual void UpdateComponent() {};

public:
	virtual void ActivateAbility() {};
		
};
