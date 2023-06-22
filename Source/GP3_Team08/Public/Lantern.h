// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Activatable.h"
#include "Lantern.generated.h"


class ULanternAbility;
class UReveal;
/**
 * 
 */
UCLASS()
class GP3_TEAM08_API ALantern : public AInteractable, public IActivatable
{
	GENERATED_BODY()
	
public:
	ALantern();

	void BeginPlay() override;
	void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void BPBeginPlay();
	UFUNCTION(BlueprintImplementableEvent)
	void BPTick();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ULanternAbility> RevealAbility;

public:
	void Activate_Implementation() override;
	void GoToNextAbility();

	UFUNCTION(CallInEditor)
	void ActivateActiveAbility();

private:
	void CreateLanternAbilities();

private:
	TArray<TObjectPtr<ULanternAbility>> Abilities;
	int CurrentAbilityIndex;
};
