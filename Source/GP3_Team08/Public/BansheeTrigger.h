// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "BansheeTrigger.generated.h"

class ABansheeAction;

UCLASS()
class GP3_TEAM08_API ABansheeTrigger : public ATriggerBox
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* OtherActor);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actions", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<ABansheeAction>> Actions;

private:
	bool Used;
};
