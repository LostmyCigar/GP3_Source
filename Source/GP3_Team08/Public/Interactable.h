// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PrismInteractable.h"
#include "Revealable.h"
#include "Interactable.generated.h"

UCLASS()
class GP3_TEAM08_API AInteractable : public AActor, public IRevealable, public IPrismInteractable
{
	GENERATED_BODY()
	
public:	
	AInteractable();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	bool BlockInteract;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	//class UStaticMeshComponent* Mesh;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Select"))
	void Select();
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Unselect"))
	void UnSelect();
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Interact"))
	void OnInteract(AActor* interactor);

	virtual void PrismInteraction_Implementation() override;
	virtual void StopPrismInteraction_Implementation() override;

	virtual void OnHide_Implementation() override;
	virtual void OnShow_Implementation() override;
};
