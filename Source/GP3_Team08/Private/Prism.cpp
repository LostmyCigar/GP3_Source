// Fill out your copyright notice in the Description page of Project Settings.


#include "Prism.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Components/ArrowComponent.h"
#include "Components/SceneComponent.h"

/* Prisms should be rewritten. 
This is working code but the system isnt pretty */

APrism::APrism()
{
	PrismBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PrismBody"));
	PrismHead = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PrismHead"));
	RaySocket = CreateDefaultSubobject<UArrowComponent>(TEXT("RayArrow"));

	PrismBody->SetupAttachment(RootComponent);
	PrismHead->SetupAttachment(PrismBody);
	RaySocket->SetupAttachment(PrismHead);

	PrimaryActorTick.bCanEverTick = true;
}

#pragma region Activation
void APrism::Activate()
{
	IsActive = true;
	OnActivate();
}

void APrism::Deactivate()
{
	IsActive = false;
	OnDeactivate();
}


#pragma endregion

#pragma region Begin/Tick
void APrism::BeginPlay()
{
	Super::BeginPlay();

	if (HeadRotationPositions.Num() > 0)
		PrismHead->SetRelativeRotation(HeadRotationPositions[RotationIndex]);
}

void APrism::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsActive || HitByBeam)
		CheckRay();
	else BeamMissed();

#if WITH_EDITOR
	DrawPrism();
#endif
}

#pragma endregion

#pragma region Rotation

void APrism::Rotate()
{
	if (IsRotating)
		return;

	int prevRotationIndex = RotationIndex;
	RotationIndex++;
	RotationIndex %= HeadRotationPositions.Num();
	OnRotate(HeadRotationPositions[prevRotationIndex], HeadRotationPositions[RotationIndex], RotationTime);
}

//Everything below this needs fixing
void APrism::OnRotate(FRotator& startRotation, FRotator& targetRotation, float duration)
{
	IsRotating = true;
	CurrentRotation = startRotation;
	TargetRotation = targetRotation;

	Timer = 0;
	GetWorldTimerManager().SetTimer(RotationTimerHandle, this, &APrism::LerpPrismHeadRotation, TickRate, true);
}

void APrism::LerpPrismHeadRotation()
{
	Timer += TickRate;
	float alpha = (Timer / RotationTime);

	if (alpha >= 1) {
		alpha = 1;
		IsRotating = false;
		GetWorldTimerManager().ClearTimer(RotationTimerHandle);
	}

	FRotator newRotation = FMath::Lerp(CurrentRotation, TargetRotation, alpha);
	PrismHead->SetRelativeRotation(newRotation);

}

#pragma endregion

#pragma region Beam
void APrism::CheckRay()
{
	FHitResult OutHit;
	//This cannot be the best way to get the world position of a scenecomponent, right?
	FVector StartPoint = RaySocket->GetComponentLocation();
	FVector EndPoint = StartPoint + RaySocket->GetForwardVector() * PrismRange;

	//Dont wanna hit this actor
	FCollisionQueryParams params(FName(TEXT("LineTrace")), true, this);
	bool hit = GetWorld()->LineTraceSingleByChannel(OutHit, StartPoint, EndPoint, ECC_Visibility, params);

	
	if (hit)
	{
		BeamHit(OutHit);
	}
	else {
		BeamMissed();
	}
}

void APrism::BeamHit(FHitResult& hitResult)
{
	OnBeamHit(hitResult);
	BeamIsHitting = true;

	if (HitInteractable && HitInteractable != hitResult.GetActor()) {
		HitPrismInteractable->Execute_PrismInteraction(HitInteractable);
	}

	//Checks if interface is implemented
	IPrismInteractable* prismInteractable = Cast<IPrismInteractable>(hitResult.GetActor());
	if (!prismInteractable)
		return;


	//save this to be able to execute interface functions wherever we want
	HitInteractable = hitResult.GetActor();
	HitPrismInteractable = prismInteractable;

	HitPrismInteractable->Execute_PrismInteraction(HitInteractable);
}

void APrism::BeamMissed()
{
	if (BeamIsHitting) {
		OnBeamStopHit();
		BeamIsHitting = false;
	}

	if (!HitInteractable || !HitPrismInteractable)
		return;

	HitPrismInteractable->Execute_StopPrismInteraction(HitInteractable);
	HitPrismInteractable = nullptr;
	HitInteractable = nullptr;
}

void APrism::PrismInteraction_Implementation()
{
	if (!HitByBeam) {
		OnHitByBeam();
		HitByBeam = true;
	}
}

void APrism::StopPrismInteraction_Implementation()
{
	if (HitByBeam) {
		OnStopHitByBeam();
		HitByBeam = false;
	}
}
#pragma endregion


#if WITH_EDITOR

void APrism::DrawPrism()
{

	//Draw active
	if (IsActive) {
		FBox ActorBounds = GetComponentsBoundingBox();

		FColor BoxColor = FColor::Blue;
		bool bPersistentLines = false;
		float LifeTime = -1.0f;

		DrawDebugBox(
			GetWorld(),
			ActorBounds.GetCenter(),
			ActorBounds.GetExtent() * 1.3f,
			FQuat::Identity,
			BoxColor,
			bPersistentLines,
			LifeTime
		);
	}

	if (HitByBeam) {
		FBox ActorBounds = GetComponentsBoundingBox();

		FColor BoxColor = FColor::Black;
		bool bPersistentLines = false;
		float LifeTime = -1.0f;

		DrawDebugBox(
			GetWorld(),
			ActorBounds.GetCenter(),
			ActorBounds.GetExtent() * 1.3f,
			FQuat::Identity,
			BoxColor,
			bPersistentLines,
			LifeTime
		);
	}

	//Draw Ray
	if (IsActive || HitByBeam) {
		FVector StartPoint = RaySocket->GetComponentLocation();
		FVector EndPoint = StartPoint + RaySocket->GetForwardVector() * PrismRange;
		DrawDebugLine(GetWorld(), StartPoint, EndPoint, FColor::Red, false, -1.0f, 0, 1.0f);

		FCollisionQueryParams params(FName(TEXT("LineTrace")), true, this);

		FHitResult OutHit;
		bool hit = GetWorld()->LineTraceSingleByChannel(OutHit, StartPoint, EndPoint, ECC_Visibility, params);
	}
}


#endif 
