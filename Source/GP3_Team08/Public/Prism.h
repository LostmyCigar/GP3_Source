// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lock.h"
#include "Prism.generated.h"

class UStaticMeshComponent;
class UArrowComponent;

UCLASS()
class GP3_TEAM08_API APrism : public ALock
{
	GENERATED_BODY()


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> PrismHead;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> PrismBody;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UArrowComponent> RaySocket;

public:
	APrism();

public:
	UPROPERTY(EditAnywhere, Category = "Rotation")
	TArray<FRotator> HeadRotationPositions;
	UPROPERTY(EditAnywhere, Category = "Rotation")
	float RotationTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Prism")
	float PrismRange;

	UPROPERTY(BlueprintReadOnly)
	bool HitByBeam;

	bool BeamIsHitting;
	UPROPERTY(BlueprintReadOnly)
	bool IsActive;

public:
	UFUNCTION(BlueprintCallable, CallInEditor)
	void Activate();
	UFUNCTION(BlueprintCallable, CallInEditor)
	void Deactivate();
	UFUNCTION(BlueprintCallable, CallInEditor)
	void Rotate();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Activate"))
	void OnActivate();
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Deactivate"))
	void OnDeactivate();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On HitByBeam"))
	void OnHitByBeam();
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On StopHitByBeam"))
	void OnStopHitByBeam();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On BeamHit"))
	void OnBeamHit(FHitResult hitResult);
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On BeamStopHit"))
	void OnBeamStopHit();

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void BeamHit(FHitResult& hitResult);
	void BeamMissed();

	void PrismInteraction_Implementation() override;
	void StopPrismInteraction_Implementation() override;

private:
	UFUNCTION()
	void OnRotate(FRotator& startRotation, FRotator& targetRotation, float duration);
	UFUNCTION()
	void LerpPrismHeadRotation();

	void CheckRay();

private:
	float TickRate = 1 / 60.0f;
	float Timer;
	int RotationIndex;
	bool IsRotating;
	

	FRotator CurrentRotation;
	FRotator TargetRotation;

	FTimerHandle RotationTimerHandle;

	//According to a UE crash log we need to save both the actor and the interface
	IPrismInteractable* HitPrismInteractable;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Prism")
	AActor* HitInteractable;


#if WITH_EDITOR
	void DrawPrism();
#endif
};
