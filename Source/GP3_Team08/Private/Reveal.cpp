// Fill out your copyright notice in the Description page of Project Settings.

#include "Reveal.h"
#include "Revealable.h"
#include "Kismet/KismetTextLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

void UReveal::Reveal(AActor* actorToShow)
{
    IRevealable* Revealable = Cast<IRevealable>(actorToShow);
    if (!Revealable)
        return;

    Revealable->Execute_OnShow(actorToShow);
}

void UReveal::StopReveal(AActor* actorToHide)
{
    IRevealable* Revealable = Cast<IRevealable>(actorToHide);
    if (!Revealable)
        return;

    Revealable->Execute_OnHide(actorToHide);
}

void UReveal::ActivateAbility()
{
	Toggle();
}

void UReveal::UpdateComponent()
{
	if (!Active)
		return;
	CheckForHiddenObjects();
}

void UReveal::Toggle()
{
	Active = !Active;

    if (!Active) {
        for (const auto& actor : CurrentlyRevealedActors)
        {
            StopReveal(actor);
        }

        CurrentlyRevealedActors.Empty();
    }

}

void UReveal::CheckForHiddenObjects()
{
    //Little bit templated cause i dont know what this is
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1));

    FVector PlayerLocation = GetOwner()->GetActorLocation();
    TArray<AActor*> IgnoreActors;
    TArray<TObjectPtr<AActor>> OutActors;

    UKismetSystemLibrary::SphereOverlapActors(
        GetWorld(),
        PlayerLocation,
        RevealRange,
        ObjectTypes,
        AActor::StaticClass(),
        IgnoreActors,
        OutActors
    );

    TSet<TObjectPtr<AActor>> NewRevealedActors;
    for (int32 i = 0; i < OutActors.Num(); i++)
    {
        if (!CurrentlyRevealedActors.Contains(OutActors[i]))
        {
            Reveal(OutActors[i]);
        }
        NewRevealedActors.Add(OutActors[i]);
    }

    for (const auto& actor : CurrentlyRevealedActors)
    {
        if (!NewRevealedActors.Contains(actor))
        {
            StopReveal(actor);
        }
    }

    CurrentlyRevealedActors = NewRevealedActors;

#if WITH_EDITOR

    for (int i = 0; i < OutActors.Num(); i++)
    {
        // Get the bounding box of the actor
        FBox ActorBounds = OutActors[i]->GetComponentsBoundingBox();

        // Draw the bounding box in the editor
        FColor BoxColor = FColor::Orange;
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

}
