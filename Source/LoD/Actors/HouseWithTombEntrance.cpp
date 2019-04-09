// Fill out your copyright notice in the Description page of Project Settings.
#include "HouseWithTombEntrance.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AHouseWithTombEntrance::AHouseWithTombEntrance()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Assign Root component
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	// Initialize Meshes
	MeshRoof = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Roof"));
	MeshRoof->SetupAttachment(Root);

	MeshHouse = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("House"));
	MeshHouse->SetupAttachment(Root);

	MeshLeftTombDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TombLeftDoor"));
	MeshLeftTombDoor->SetupAttachment(Root);

	MeshRightTombDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TombRightDoor"));
	MeshRightTombDoor->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AHouseWithTombEntrance::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHouseWithTombEntrance::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

