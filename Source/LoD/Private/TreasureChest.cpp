// Fill out your copyright notice in the Description page of Project Settings.

#include "TreasureChest.h"
#include "References.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "LoDGameMode.h"
#include "TargetingSystem.h"
#include "ItemSpawner.h"
#include "../LoDEnumTypes.h"
#include "../LoDCharacter.h"

// Sets default values
ATreasureChest::ATreasureChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh_BottomPart = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh_BottomPart"));
	Mesh_BottomPart->SetupAttachment(Root);

	Mesh_TopPart = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh_TopPart"));
	Mesh_TopPart->SetupAttachment(Mesh_BottomPart);
}

// Called when the game starts or when spawned
void ATreasureChest::BeginPlay()
{
	Super::BeginPlay();

	/** Called when the mouse cursor is moved over this actor if mouse over events are enabled in the player controller. */
	Mesh_BottomPart->OnBeginCursorOver.AddDynamic(this, &ATreasureChest::OutlineObject);
	Mesh_TopPart->OnBeginCursorOver.AddDynamic(this, &ATreasureChest::OutlineObject);

	/** Called when the mouse cursor is moved off this actor if mouse over events are enabled in the player controller. */
	Mesh_BottomPart->OnEndCursorOver.AddDynamic(this, &ATreasureChest::CancelOutlineObject);
	Mesh_TopPart->OnEndCursorOver.AddDynamic(this, &ATreasureChest::CancelOutlineObject);

	/** Bind the mouse event, on mouse click, set the pick up item, to character targeted item */
	OnReleased.AddDynamic(this, &ATreasureChest::SetToTarget);
}

void ATreasureChest::SetToTarget(AActor * TouchedActor, FKey ButtonPressed)
{
	//if (GEngine != NULL) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Item was targeted: " + name));
	ptrCharacter->GetTargetingSystem()->SetTarget(this, ETargetType::TREASURE_CHEST);
}

void ATreasureChest::RemoveDynamics()
{
	Mesh_BottomPart->OnBeginCursorOver.RemoveDynamic(this, &ATreasureChest::OutlineObject);
	Mesh_BottomPart->OnBeginCursorOver.RemoveDynamic(this, &ATreasureChest::CancelOutlineObject);

	Mesh_TopPart->OnEndCursorOver.RemoveDynamic(this, &ATreasureChest::OutlineObject);
	Mesh_TopPart->OnEndCursorOver.RemoveDynamic(this, &ATreasureChest::CancelOutlineObject);

	OnReleased.RemoveDynamic(this, &ATreasureChest::SetToTarget);

	ptrCharacter->GetTargetingSystem()->ClearTarget();

	Mesh_TopPart->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh_BottomPart->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// try and play the sound if specified
	if (AC_OpenChest)
	{
		UGameplayStatics::PlaySoundAtLocation(this, AC_OpenChest, GetActorLocation());
	}

	uint8 numberOfItems = FMath::RandRange(1, 6);

	for (uint8 i = 0; i < numberOfItems; i++)
	{
		FVector spawnLocation = this->GetActorRightVector() * FMath::RandRange(-70, 70);
		ptrGameMode->itemSpawner->SpawnItem((this->GetActorLocation() + (this->GetActorForwardVector() * FMath::RandRange(70, 150)) - spawnLocation), FRotator(0.0f, FMath::RandRange(0.0f, 360.0f), 0.0f));
	}
}


void ATreasureChest::OutlineObject(UPrimitiveComponent * TouchedComponent)
{
	Mesh_BottomPart->SetRenderCustomDepth(true);
	Mesh_TopPart->SetRenderCustomDepth(true);
}

void ATreasureChest::CancelOutlineObject(UPrimitiveComponent * TouchedComponent)
{
	Mesh_BottomPart->SetRenderCustomDepth(false);
	Mesh_TopPart->SetRenderCustomDepth(false);
}