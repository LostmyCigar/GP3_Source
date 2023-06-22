// Fill out your copyright notice in the Description page of Project Settings.


#include "StationaryPrism.h"
#include "GP3_Team08/GP3_Team08Character.h"

void AStationaryPrism::GiveLantern(AActor* interactor)
{
	auto Character = Cast<AGP3_Team08Character>(interactor);
	if (!Character || !Lantern)
		return;

	Character->OnTakeObject(this, Lantern);
}

//Should be references not pointers here
void AStationaryPrism::HoldLantern(AActor* interactor, USceneComponent* parent)
{
	auto Character = Cast<AGP3_Team08Character>(interactor);
	if (!Character)
		return;

	auto lantern = Cast<ALantern>(Character->GetHeldObject());
	if (!lantern)
		return;

	Character->OnPlaceHeldObject(this, PrismHead, LanternSocketName);
}

void AStationaryPrism::ReceiveLantern(ALantern* LanternToHold)
{
	IsHoldingLantern = true;
	Lantern = LanternToHold;
	Activate();
}

void AStationaryPrism::RemoveLantern()
{
	if (IsHoldingLantern || Lantern) {
		IsHoldingLantern = false;
		Lantern = nullptr;
		Deactivate();
	}
}
