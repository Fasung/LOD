// Fill out your copyright notice in the Description page of Project Settings.

#include "GameSettings.h"
#include "References.h"

// Sets default values
AGameSettings::AGameSettings()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	ControlsSettings = CreateDefaultSubobject<UControlsSettings>(TEXT("ControlSettings"));

	VideoSettings = CreateDefaultSubobject<UVideoSettings>(TEXT("VideoSettings"));

	ptrSettings = this;
}

// Called when the game starts or when spawned
void AGameSettings::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGameSettings::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

