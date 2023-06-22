// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "Lock.h"

// Sets default values
ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = false;

}


void ADoor::BeginPlay()
{
	Super::BeginPlay();


	if (!OpenManually)
	{
		for (int i = 0; i < Locks.Num(); i++)
		{
			if (Locks[i]) //This should never be a nullptr but i aint taking any risks
				Locks[i]->OnUnlock().AddUObject(this, &ADoor::TryOpenDoor);
		}
	}

	if (ShouldCloseWhenLocked) {
		for (int i = 0; i < Locks.Num(); i++)
		{
			if (Locks[i])
				Locks[i]->OnLock().AddUObject(this, &ADoor::TryCloseDoor);
		}
	}
}

void ADoor::ForceOpenDoor()
{
	OpenDoor();
}

void ADoor::ForceCloseDoor()
{
	CloseDoor();
}

void ADoor::TryOpenDoor()
{
	//We cant open doors twice in a row, right?
	if (IsOpen)
		return;

	if (OnlyOneLockNeeded && CheckForOneLock()) {
		OpenDoor();
		return;
	}

	if (CheckAllLocks()) {
		OpenDoor();
	}
}

void ADoor::TryCloseDoor()
{
	//We cant close already closed doors either
	if (!IsOpen)
		return;

	if (!CheckAllLocks() && ShouldCloseWhenLocked) {
		CloseDoor();
	}
}

void ADoor::TryOpenClose()
{
	switch (IsOpen)
	{
	case true:
		TryCloseDoor();
		break;

	case false:
		TryOpenDoor();
		break;
	}
}

void ADoor::ForceOpenClose()
{
	switch (IsOpen)
	{
	case true:
		CloseDoor();
		break;

	case false:
		OpenDoor();
		break;
	}
}


void ADoor::OpenDoor()
{
	IsOpen = true;
	OnOpenDoor();
}

void ADoor::CloseDoor()
{
	IsOpen = false;
	OnCloseDoor();
}


bool ADoor::CheckAllLocks()
{
	for (int i = 0; i < Locks.Num(); i++)
	{
		if (!Locks[i]->Unlocked)
			return false;
	}
	return true;
}

bool ADoor::CheckForOneLock()
{
	for (int i = 0; i < Locks.Num(); i++)
	{
		if (Locks[i]->Unlocked)
			return true;
	}
	return false;
}





