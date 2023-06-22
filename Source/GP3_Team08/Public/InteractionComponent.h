// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

class AInteractable;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GP3_TEAM08_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInteractionComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	float InteractionRange;


public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void InteractWithClosestInteractable();

private:

	AInteractable* SelectedInteractable;
	AInteractable* GetClosestInteractable();


#if WITH_EDITOR
	void DrawInteraction();
#endif
};
