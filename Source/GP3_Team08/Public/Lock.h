// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "Lock.generated.h"



UCLASS()
class GP3_TEAM08_API ALock : public AInteractable
{
	GENERATED_BODY()
	
public:	
	ALock();

	DECLARE_EVENT(ALock, UnlockEvent);
	UnlockEvent& OnUnlock() { return OnUnlockEvent; };
	DECLARE_EVENT(ALock, LockEvent);
	LockEvent& OnLock() { return OnLockEvent; };

	UFUNCTION(BlueprintCallable, CallInEditor, meta = (ShortToolTip = "Don't use in begin play"))
	void Unlock();
	UFUNCTION(BlueprintCallable, CallInEditor, meta = (ShortToolTip = "Don't use in begin play"))
	void Lock();

	bool Unlocked;

private:
	UnlockEvent OnUnlockEvent;
	LockEvent OnLockEvent;
};
