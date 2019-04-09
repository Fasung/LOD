// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "LoDGameMode.h"
#include "LoDPlayerController.h"
#include "LoDCharacter.h"
#include "Settings/GameSettings.h"
#include "References.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "ItemSpawner.h"
#include "Engine.h"
#include "Runtime/Engine/Classes/Materials/Material.h"
#include "Managers/ManagerNPC.h"

ALoDCharacter * ptrCharacter;
ALoDGameMode * ptrGameMode;
AGameSettings * ptrSettings;
ALoDPlayerController * ptrPlayerController;
ALoDHUD * ptrHUD;


void ALoDGameMode::BeginPlay()
{
	Super::BeginPlay();


	if (PlayerHudClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHudClass);
	}
	 
	CurrentWidget->AddToViewport();
}

ALoDGameMode::ALoDGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}


	itemSpawner = CreateDefaultSubobject<UItemSpawner>(TEXT("ItemSpawner"));


	managerNPC = CreateDefaultSubobject<UManagerNPC>(TEXT("ManagerNPC"));

	ptrGameMode = this;
}
