// Fill out your copyright notice in the Description page of Project Settings.

#include "Lantern.h"
#include "LanternAbility.h"
#include "Reveal.h"


ALantern::ALantern()
{
	PrimaryActorTick.bCanEverTick = true;
	CreateLanternAbilities();
}

void ALantern::CreateLanternAbilities()
{
	RevealAbility = CreateDefaultSubobject<UReveal>(TEXT("RevealAbility"));
}

void ALantern::BeginPlay()
{
	Super::BeginPlay();

	Abilities.Add(RevealAbility);
	BPBeginPlay();
}

void ALantern::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Abilities[CurrentAbilityIndex]) {
		Abilities[CurrentAbilityIndex]->UpdateComponent();
	}

	BPTick();
}

void ALantern::Activate_Implementation()
{
	if (Abilities.IsValidIndex(CurrentAbilityIndex)) {
		Abilities[CurrentAbilityIndex]->ActivateAbility();
	}
}

void ALantern::GoToNextAbility()
{
	CurrentAbilityIndex++;
	CurrentAbilityIndex %= Abilities.Num();
}

void ALantern::ActivateActiveAbility()
{
	Activate_Implementation();
}



