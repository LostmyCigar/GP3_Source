// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "Door.generated.h"

class ALock;

UCLASS()
class GP3_TEAM08_API ADoor : public AInteractable
{
	GENERATED_BODY()
	
public:	

	ADoor();

protected:

	virtual void BeginPlay() override;

public:	

	UFUNCTION(BlueprintCallable)
	void ForceOpenDoor();
	UFUNCTION(BlueprintCallable)
	void ForceCloseDoor();
	UFUNCTION(BlueprintCallable)
	void ForceOpenClose();

	UFUNCTION(BlueprintCallable)
	void TryOpenDoor();
	UFUNCTION(BlueprintCallable)
	void TryCloseDoor();
	UFUNCTION(BlueprintCallable)
	void TryOpenClose();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On OpenDoor"))
	void OnOpenDoor();
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On CloseDoor"))
	void OnCloseDoor();
	
	UPROPERTY(EditAnywhere, Category = "Locks")
	TArray<ALock*> Locks;

	UPROPERTY(EditAnywhere, Category = "Locks")
	bool ShouldCloseWhenLocked;
	UPROPERTY(EditAnywhere, Category = "Locks")
	bool OnlyOneLockNeeded;

	UPROPERTY(EditAnywhere, Category = "Locks")
	bool OpenManually;

private:
	void OpenDoor();
	void CloseDoor();

	bool IsOpen;

	bool CheckAllLocks();
	bool CheckForOneLock();
};
