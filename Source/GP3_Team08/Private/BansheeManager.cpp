// Fill out your copyright notice in the Description page of Project Settings.

#include "BansheeManager.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialParameterCollection.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "GP3_Team08\GP3_Team08Character.h"

// Sets default values
ABansheeManager::ABansheeManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABansheeManager::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<AGP3_Team08Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (!Player)
		return;

	UpdateLightsourcePositions();

}

void ABansheeManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Player)
		return;

	CheckFogStatus();
	UpdateLightsourcePositions();
	HandlePlayerInFogEvents(DeltaTime);
	CheckTimers(DeltaTime);

	PlayerInFogLastFrame = InFog;
	PlayerInSoftFogLastFrame = InSoftFog;
}

#pragma region GetLightsource
//Somehting when wrong with the architecture for this to become they way we check for Lightsourcepositions
/*
* We should only really have to update the position of a lightsource whenever it changes
* When that happens we only want to change the one that has a new position
* Currently we are checking for change and updating all positions, every frame
*
* I dont think we cant come up with a better solution right now
* But this is mega cursed
*/

//This was not updating properly, remade it in BP exactly the same and it worked, somehow
//void ABansheeManager::UpdateLightsourcePositions()
//{
//	LightsourcePositions.Empty();
//	TArray<FName> parameternames = FirePositions->getvec
//	
//	for (int i = 0; i < parameternames.Num(); i++)
//	{
//		FLinearColor ParameterValue;
//		auto value = FirePositions->GetVectorParameterValue(parameternames[i], ParameterValue);
//		LightsourcePositions.Add(FVector(value.R, value.G, value.B));
//	}
//
//	//for (int i = 0; i < parameters.Num(); i++)
//	//{
//	//	auto color = parameters[i].DefaultValue;
//	//	FVector paramPosition = FVector(color.R, color.G, color.B);
//	//	LightsourcePositions.Add(paramPosition);	
//	//}
//}

//void ABansheeManager::UpdateLightsourcePositions()
//{
//	LightsourcePositions.Empty();
//	TArray<FName> parameternames = FirePositions->GetVectorParameterNames();
//
//	for (int i = 0; i < parameternames.Num(); i++)
//	{
//		auto value = UKismetMaterialLibrary::GetVectorParameterValue(GetWorld(), FirePositions, parameternames[i]);
//		LightsourcePositions.Add(FVector(value.R, value.G, value.B));
//	}
//}
#pragma endregion


float ABansheeManager::ClosestLightsourceDistance()
{
	//Should not be invalid but im not risking it
	if (!LightsourcePositions.IsValidIndex(0))
		return RangeToLightsource * 2;

	FVector playerLocation = Player->GetActorLocation();
	float distance = FVector::Dist(LightsourcePositions[0], playerLocation);
	for (int i = 1; i < LightsourcePositions.Num(); i++) //Start from 1 since we checked index 0 already
	{
		float thisDistance = FVector::Dist(LightsourcePositions[i], playerLocation);
		if (thisDistance < distance) {
			distance = thisDistance;
		}
	}

	return distance;
}

void ABansheeManager::CheckFogStatus()
{
	if (InsideFogVolume <= 0) {
		InSoftFog = false;
		InFog = false;
		InAnyFog = false;
		return;
	}

	float closestSource = ClosestLightsourceDistance();
	if (RangeToLightsource < closestSource) {
		InAnyFog = true;

		if (closestSource < RangeToLightsource + SoftRangeToLightsourceExtension) {
			InSoftFog = true;
			InFog = false;
		}
		else
		{
			InSoftFog = false;
			InFog = true;
		}
	}
	else
	{
		InSoftFog = false;
		InFog = false;
		InAnyFog = false;
	}
}

void ABansheeManager::HandlePlayerInFogEvents(float deltaTime)
{
	CheckSoftFog(deltaTime);
	CheckFog(deltaTime);
	if (InAnyFog)
		OnInsideAnyFog(InFog);
}

void ABansheeManager::CheckFog(float deltaTime)
{
	if (InFog) {

		switch (PlayerInFogLastFrame)
		{
		case true:
			OnInsideFog(FogTimer);
			break;
		case false:
			OnEnterFog();
			break;
		}
	}
	else if (PlayerInFogLastFrame) {
		OnExitFog();
	}
}

void ABansheeManager::CheckSoftFog(float deltaTime)
{
	if (InSoftFog) {
		switch (PlayerInSoftFogLastFrame)
		{
		case true:
			OnSoftInsideFog(FogTimer, SoftFogOnlyTimer);
			break;
		case false:
			OnSoftEnterFog();
			break;
		}
	}
	else if (PlayerInSoftFogLastFrame) {
		OnSoftExitFog();
	}
}

void ABansheeManager::CheckTimers(float deltaTime)
{
	if (!InAnyFog) {
		if (FogTimer > 0) {
			FogTimer -= deltaTime * DecreaseMultiplier;
		}
		SoftFogOnlyTimer = 0;
	}
	else if (InSoftFog) {
		FogTimer += deltaTime * SoftInFogTimerMulitplier;
		SoftFogOnlyTimer += deltaTime;
	}
	else if (InFog) {
		FogTimer += deltaTime;
	}
}


