// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BansheeManager.generated.h"

class AGP3_Team08Character;

UCLASS()
class GP3_TEAM08_API ABansheeManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ABansheeManager();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fog")
	float RangeToLightsource;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fog")
	float SoftRangeToLightsourceExtension;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fog")
	float DecreaseMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fog")
	float SoftInFogTimerMulitplier;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fog")
	float SoftFogOnlyTimer;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fog")
	float FogTimer;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player")
	AGP3_Team08Character* Player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fog")
	UMaterialParameterCollection* FirePositions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fog")
	TArray<FVector> LightsourcePositions;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fog")
	int InsideFogVolume;

	bool PlayerInFogLastFrame;
	bool PlayerInSoftFogLastFrame;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fog")
	bool InAnyFog;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fog")
	bool InSoftFog;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fog")
	bool InFog;


public:
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On EnterFog"))
	void OnEnterFog();
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On LeaveFog"))
	void OnExitFog();
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On InsideFog"))
	void OnInsideFog(float timeInsideFog);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On InsideAnyFog"))
	void OnInsideAnyFog(float timeInsideFog);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On SoftEnterFog"))
	void OnSoftEnterFog();
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On SoftExitFog"))
	void OnSoftExitFog();
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On SoftInsideFog"))
	void OnSoftInsideFog(float timeInsideFog, float timeInsideSoftFog);



public:
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On UpdateLightSource"))
	void UpdateLightsourcePositions();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	// void UpdateLightsourcePositions();
	float ClosestLightsourceDistance();
	void CheckFogStatus();
	void HandlePlayerInFogEvents(float deltaTime);
	void CheckFog(float deltaTime);
	void CheckSoftFog(float deltaTime);
	void CheckTimers(float deltaTime);
};
