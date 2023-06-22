// Fill out your copyright notice in the Description page of Project Settings.

#include "BansheeTrigger.h"
#include "BansheeAction.h"

void ABansheeTrigger::OnOverlap(AActor* OtherActor)
{
	if (!OtherActor->IsA(APawn::StaticClass()) || Used)
		return;

	//Should maybe add option to have a delay between actions
	for (int i = 0; i < Actions.Num(); i++)
	{
		Actions[i]->Activate();
	}
	Used = true;
}

