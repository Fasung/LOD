// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemSpawner.h"
#include "Engine/World.h"
#include "../Components/ShopSystem.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"

// Sets default values for this component's properties
UItemSpawner::UItemSpawner()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bTickEvenWhenPaused = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	// ...
}


void UItemSpawner::SpawnItem(FVector Location, FRotator Rotation)
{
	FActorSpawnParameters SpawnInfo;
	
	dropChance = FMath::RandRange(0, 100);
	if (dropChance < 15) GetWorld()->SpawnActor<AItem>(this->armorsArray[FMath::RandRange(0, this->armorsArray.Num() - 1)], Location, Rotation, SpawnInfo);
	else if (dropChance < 30) GetWorld()->SpawnActor<AItem>(this->shieldsArray[FMath::RandRange(0, this->shieldsArray.Num() - 1)], Location, Rotation, SpawnInfo);
	else if (dropChance < 45) GetWorld()->SpawnActor<AItem>(this->weaponsArray[FMath::RandRange(0, this->weaponsArray.Num() - 1)], Location, Rotation, SpawnInfo);
	else if (dropChance < 60) GetWorld()->SpawnActor<AItem>(this->potionsArray[FMath::RandRange(0, this->potionsArray.Num() - 1)], Location, Rotation, SpawnInfo);
	//else if (dropChance < 50) GetWorld()->SpawnActor<AItem>(spellsArray[FMath::RandRange(0, spellsArray.Num() - 1)], Location, Rotation, SpawnInfo);
	else if (dropChance < 70) GetWorld()->SpawnActor<AItem>(this->gold, Location, Rotation, SpawnInfo);
	else if (dropChance < 85) GetWorld()->SpawnActor<AItem>(this->silver, Location, Rotation, SpawnInfo);
	else GetWorld()->SpawnActor<AItem>(this->bronze, Location, Rotation, SpawnInfo);
}

void UItemSpawner::GenerateAlchemistItems(UShopItems * shopItems, uint8 itemsStrongness)
{
	shopItems->bHasPotions = true;
	shopItems->bHasSpells = true;

	loopTime = FMath::RandRange(25, 60);
	for (uint8 i = 0; i < loopTime; i++)
	{
		FActorSpawnParameters SpawnInfo;
		AItem * newItem = GetWorld()->SpawnActor<AItem>(this->potionsArray[FMath::RandRange(0, this->potionsArray.Num() - 1)], FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo);
		// Dissable object visiblity
		newItem->Mesh->SetVisibility(false);
		newItem->MeshAdditional->SetVisibility(false);
		newItem->itemNameWidget->SetVisibility(false);
		// Dissable collision with mouse
		newItem->Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		newItem->MeshAdditional->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		newItem->itemNameWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		shopItems->itemsPotions.Add(newItem);
	}
	
	
	/*
	loopTime = FMath::RandRange(10, 30);
	for (uint8 i = 0; i < loopTime; i++)
	{
		FActorSpawnParameters SpawnInfo;
		AItem * newItem = GetWorld()->SpawnActor<AItem>(spellsArray[FMath::RandRange(0, spellsArray.Num() - 1)], FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo);
		// Dissable object visiblity
		newItem->Mesh->SetVisibility(false);
		newItem->MeshAdditional->SetVisibility(false);
		newItem->itemNameWidget->SetVisibility(false);
		// Dissable collision with mouse
		newItem->Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		newItem->MeshAdditional->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		newItem->itemNameWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		shopItems->itemsSpells.Add(newItem);
	}
	*/
}

void UItemSpawner::GenerateBlacksmithItems(UShopItems * shopItems, uint8 itemsStrongness)
{
	shopItems->bHasArmors = true;
	shopItems->bHasJewelry = true;
	shopItems->bHasShields = true;
	shopItems->bHasWeapons = true;

	// Generate armors
	loopTime = FMath::RandRange(10, 30);
	for (uint8 i = 0; i < loopTime; i++)
	{
		FActorSpawnParameters SpawnInfo;
		AItem * newItem = GetWorld()->SpawnActor<AItem>(this->armorsArray[FMath::RandRange(0, this->armorsArray.Num() - 1)], FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo);
		// Dissable object visiblity
		newItem->Mesh->SetVisibility(false);
		newItem->MeshAdditional->SetVisibility(false);
		newItem->itemNameWidget->SetVisibility(false);
		// Dissable collision with mouse
		newItem->Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		newItem->MeshAdditional->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		newItem->itemNameWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		shopItems->itemsArmors.Add(newItem);
	}

	// Generate jewelry
	loopTime = FMath::RandRange(10, 30);
	for (uint8 i = 0; i < loopTime; i++)
	{
		FActorSpawnParameters SpawnInfo;
		AItem * newItem = GetWorld()->SpawnActor<AItem>(this->jewelryArray[FMath::RandRange(0, this->jewelryArray.Num() - 1)], FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo);
		// Dissable object visiblity
		newItem->Mesh->SetVisibility(false);
		newItem->MeshAdditional->SetVisibility(false);
		newItem->itemNameWidget->SetVisibility(false);
		// Dissable collision with mouse
		newItem->Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		newItem->MeshAdditional->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		newItem->itemNameWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		shopItems->itemsJewelry.Add(newItem);
	}

	// Generate shields
	loopTime = FMath::RandRange(10, 30);
	for (uint8 i = 0; i < loopTime; i++)
	{
		FActorSpawnParameters SpawnInfo;
		AItem * newItem = GetWorld()->SpawnActor<AItem>(this->shieldsArray[FMath::RandRange(0, this->shieldsArray.Num() - 1)], FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo);
		// Dissable object visiblity
		newItem->Mesh->SetVisibility(false);
		newItem->MeshAdditional->SetVisibility(false);
		newItem->itemNameWidget->SetVisibility(false);
		// Dissable collision with mouse
		newItem->Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		newItem->MeshAdditional->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		newItem->itemNameWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		shopItems->itemsShields.Add(newItem);
	}

	// Generate weapons
	loopTime = FMath::RandRange(10, 30);
	for (uint8 i = 0; i < loopTime; i++)
	{
		FActorSpawnParameters SpawnInfo;
		AItem * newItem = GetWorld()->SpawnActor<AItem>(this->weaponsArray[FMath::RandRange(0, this->weaponsArray.Num() - 1)], FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo);
		// Dissable object visiblity
		newItem->Mesh->SetVisibility(false);
		newItem->MeshAdditional->SetVisibility(false);
		newItem->itemNameWidget->SetVisibility(false);
		// Dissable collision with mouse
		newItem->Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		newItem->MeshAdditional->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		newItem->itemNameWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		shopItems->itemsWeapons.Add(newItem);
	}
}

