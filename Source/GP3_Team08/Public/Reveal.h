// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LanternAbility.h"
#include "Reveal.generated.h"

/**
 * 
 */
UCLASS()
class GP3_TEAM08_API UReveal : public ULanternAbility
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float RevealRange;

public:
	void Reveal(AActor* actorToShow);
	void StopReveal(AActor* actorToHide);

public:
	void ActivateAbility() override;
	void UpdateComponent() override;

private:
	void Toggle();
	void CheckForHiddenObjects();

public:
	bool Active;

private:
	TSet<TObjectPtr<AActor>> CurrentlyRevealedActors;
};
