// Fill out your copyright notice in the Description page of Project Settings.

#include "Torch.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ATorch::ATorch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	// Initialize torch mesh and add it into root
	MeshTorch = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Torch"));
	MeshTorch->SetupAttachment(Root);

	// Initialize torch point light and add it into root
	LightEffect = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
	LightEffect->SetupAttachment(Root);

}

// Called when the game starts or when spawned
void ATorch::BeginPlay()
{
	Super::BeginPlay();
}
