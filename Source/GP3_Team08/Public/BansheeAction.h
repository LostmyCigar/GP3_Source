// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BansheeAction.generated.h"

UCLASS()
class GP3_TEAM08_API ABansheeAction : public AActor
{
	GENERATED_BODY()
	
public:	
	ABansheeAction();

public:	
	UFUNCTION(BlueprintCallable)
	virtual void Activate();

	UFUNCTION(BlueprintImplementableEvent)
	void OnActivate();
};
