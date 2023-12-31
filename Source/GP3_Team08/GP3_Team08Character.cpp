// Copyright Epic Games, Inc. All Rights Reserved.

#include "GP3_Team08Character.h"
#include "GP3_Team08GameMode.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Public/LanternHolder.h"
#include "Activatable.h"
#include "Interactable.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InteractionComponent.h"


//////////////////////////////////////////////////////////////////////////
// AGP3_Team08Character

AGP3_Team08Character::AGP3_Team08Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	InteractionComponent = CreateAbstractDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AGP3_Team08Character::BeginPlay()
{
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

#pragma region Input

void AGP3_Team08Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGP3_Team08Character::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGP3_Team08Character::Look);

		//Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AGP3_Team08Character::Interact);

		//Drop held item
		EnhancedInputComponent->BindAction(DropAction, ETriggerEvent::Triggered, this, &AGP3_Team08Character::Drop);

		//Activate Ability
		EnhancedInputComponent->BindAction(AbilityAction, ETriggerEvent::Triggered, this, &AGP3_Team08Character::ActivateAbility);
	}
}


void AGP3_Team08Character::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AGP3_Team08Character::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AGP3_Team08Character::Interact()
{
	InteractionComponent->InteractWithClosestInteractable();
}

void AGP3_Team08Character::Drop()
{
	DropHeldObject();
}

void AGP3_Team08Character::ActivateAbility()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("Activate"));
	if (!HeldObject)
		return;
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("HeldObject found"));

	IActivatable* activatable = Cast<IActivatable>(HeldObject);
	if (activatable) {
		activatable->Execute_Activate(HeldObject);
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("Activate held object"));
	}

}

#pragma endregion

#pragma region Picking/Placing/Dropping

void AGP3_Team08Character::HoldObject(AInteractable* itemToHold)
{
	DropHeldObject();
	HeldObject = itemToHold;
	HeldObject->BlockInteract = true;
	IsHoldingObject = true;

	//Hardcoded socket is a bit iffy but please ignore
	HeldObject->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Lantern_Socket"));
}

void AGP3_Team08Character::DropHeldObject()
{
	if (!HeldObject)
		return;

	HeldObject->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	HeldObject->BlockInteract = false;
	HeldObject = nullptr;
	IsHoldingObject = false;
}

/*
We could make this return AInteractable* HeldObject 
Incase we want to check what the player is holding without having to use GetHeldObject() first 
*/
void AGP3_Team08Character::PlaceHeldObject(AActor* NewOwner, USceneComponent* Parent, FName SocketName)
{
	if (!HeldObject)
		return;

	ILanternHolder* holder = Cast<ILanternHolder>(NewOwner);
	ALantern* lantern = Cast<ALantern>(HeldObject);
	if (holder && lantern)
		holder->ReceiveLantern(lantern);

	HeldObject->AttachToComponent(Parent, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
	HeldObject = nullptr;
	IsHoldingObject = false;
}

#pragma endregion

void AGP3_Team08Character::Die()
{
	OnDeathEvent.Broadcast();
}


