// Fill out your copyright notice in the Description page of Project Settings.


#include "Lock.h"

// Sets default values
ALock::ALock()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ALock::Unlock()
{
	Unlocked = true;
	OnUnlockEvent.Broadcast();
}

void ALock::Lock()
{
	Unlocked = false;
	OnLockEvent.Broadcast();
}


