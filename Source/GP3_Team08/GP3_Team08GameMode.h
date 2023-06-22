// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GP3_Team08GameMode.generated.h"

UCLASS(minimalapi)
class AGP3_Team08GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGP3_Team08GameMode();

public:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void SetPlayerLocationAsCheckPoint();
	UFUNCTION(BlueprintCallable)
	void SetCheckPoint(FVector newLocation);
	UFUNCTION(BlueprintCallable)
	void MovePlayerToLastCheckPoint();

public:
	UPROPERTY(BluePrintReadWrite)
	FVector PlayerCheckPoint;

private:
	class AGP3_Team08Character* Player;
	
};



