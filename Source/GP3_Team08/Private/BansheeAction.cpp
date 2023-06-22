// Fill out your copyright notice in the Description page of Project Settings.


#include "BansheeAction.h"


ABansheeAction::ABansheeAction()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABansheeAction::Activate()
{
	OnActivate();
}


