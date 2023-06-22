// Copyright Epic Games, Inc. All Rights Reserved.

#include "GP3_Team08GameMode.h"
#include "GP3_Team08Character.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AGP3_Team08GameMode::AGP3_Team08GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AGP3_Team08GameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGP3_Team08GameMode::BeginPlay()
{
	Super::BeginPlay();
	Player = CastChecked<AGP3_Team08Character>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	Player->OnDeath().AddUObject(this, &AGP3_Team08GameMode::MovePlayerToLastCheckPoint);
	SetPlayerLocationAsCheckPoint();

}

void AGP3_Team08GameMode::SetPlayerLocationAsCheckPoint()
{
	if (Player)
		PlayerCheckPoint = Player->GetActorLocation();
}

void AGP3_Team08GameMode::SetCheckPoint(FVector newLocation)
{
	PlayerCheckPoint = newLocation;
}

void AGP3_Team08GameMode::MovePlayerToLastCheckPoint()
{
	if (Player)
		Player->SetActorLocation(PlayerCheckPoint);
}


