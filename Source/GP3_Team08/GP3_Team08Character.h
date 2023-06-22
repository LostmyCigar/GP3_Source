// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "GP3_Team08Character.generated.h"

class UInteractionComponent;
class AInteractable;

UCLASS(config=Game)
class AGP3_Team08Character : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Drop Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* DropAction;

	/**Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InteractAction;

	/** Ability Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AbilityAction;

public:
	AGP3_Team08Character();

	//Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interaction, meta = (AllowPrivateAccess = "true"))
	UInteractionComponent* InteractionComponent;
	
protected:

	//Input
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Interact();
	void Drop();
	void ActivateAbility();

public:
	DECLARE_EVENT(AGP3_Team08Character, DeathEvent);
	DeathEvent& OnDeath() { return OnDeathEvent; };

	UFUNCTION(BlueprintCallable, CallInEditor)
	void Die();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnPlaceHeldObject(AActor* NewOwner, USceneComponent* Parent, FName SocketName);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnTakeObject(AActor* oldHolder, AInteractable* itemToHold);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnLightBonfire();


	UFUNCTION(BlueprintCallable)
	void HoldObject(AInteractable* itemToHold);
	UFUNCTION(BlueprintCallable, CallInEditor)
	void DropHeldObject();

	UFUNCTION(BlueprintCallable)
	void PlaceHeldObject(AActor* NewOwner, USceneComponent* Parent, FName SocketName = NAME_None);


protected:

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay();

private:
	AInteractable* HeldObject;
	DeathEvent OnDeathEvent;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool IsHoldingObject;

public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	//Can return nullptr
	UFUNCTION(BlueprintCallable)
	FORCEINLINE class AInteractable* GetHeldObject() const { return HeldObject; }
};

