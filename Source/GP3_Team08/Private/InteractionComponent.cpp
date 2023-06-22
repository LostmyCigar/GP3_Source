// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractionComponent.h"
#include "Interactable.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/Vector.h"


// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

#if WITH_EDITOR
    DrawInteraction();
#endif 


    if (GetClosestInteractable() == SelectedInteractable) 
        return;

    if (SelectedInteractable)
        SelectedInteractable->UnSelect();

    SelectedInteractable = GetClosestInteractable();

    if (SelectedInteractable)
        SelectedInteractable->Select();


}


//Should be called on player input from the component owner
void UInteractionComponent::InteractWithClosestInteractable()
{
    if (SelectedInteractable)
        SelectedInteractable->OnInteract(GetOwner());
}

AInteractable* UInteractionComponent::GetClosestInteractable()
{
    //Little bit templated cause i dont know what this is
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1));

    //Now i know what im doing again B)
    FVector PlayerLocation = GetOwner()->GetActorLocation();
    TArray<AActor*> IgnoreActors;
    TArray<AActor*> OutActors;

    //Gets all interactables in range and but them in a list
    UKismetSystemLibrary::SphereOverlapActors(
        GetWorld(),
        PlayerLocation,
        InteractionRange,
        ObjectTypes,
        AInteractable::StaticClass(),
        IgnoreActors,
        OutActors
    );

    //Looks for closest obect in the list and returns it
    AInteractable* ClosestInteractable = nullptr;
    float ClosestRange = InteractionRange * InteractionRange + 100; //+100 for good messure
    for (int i = 0; i < OutActors.Num(); i++)
    {
        AInteractable* interactable = Cast<AInteractable>(OutActors[i]);
        if (interactable->BlockInteract)
            continue;

        float dist = FVector::DistSquared(PlayerLocation, interactable->GetActorLocation());
        if (dist < ClosestRange) {
            ClosestRange = dist;
            ClosestInteractable = interactable;
        } 
    }

    return ClosestInteractable;
}



#if WITH_EDITOR

void UInteractionComponent::DrawInteraction()
{
    DrawDebugSphere(GetWorld(), GetOwner()->GetActorLocation(), InteractionRange, 1, FColor::Cyan);

    //Good to avoid memory access violations
    if (!GetClosestInteractable())
        return;

    // Get the bounding box of the actor
    FBox ActorBounds = GetClosestInteractable()->GetComponentsBoundingBox();

    // Draw the bounding box in the editor
    FColor BoxColor = FColor::Green; 
    bool bPersistentLines = false; 
    float LifeTime = -1.0f; 

    DrawDebugBox(
        GetWorld(),
        ActorBounds.GetCenter(),
        ActorBounds.GetExtent() * 1.2f,
        FQuat::Identity,
        BoxColor,
        bPersistentLines,
        LifeTime
    );


}


#endif 
