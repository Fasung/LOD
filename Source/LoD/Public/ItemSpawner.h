// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item.h"
#include "ItemSpawner.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LOD_API UItemSpawner : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Items")
	TArray<TSubclassOf<AItem>> armorsArray;

	UPROPERTY(EditAnywhere, Category = "Items")
	TArray<TSubclassOf<AItem>> jewelryArray;

	UPROPERTY(EditAnywhere, Category = "Items")
	TArray<TSubclassOf<AItem>> potionsArray;

	UPROPERTY(EditAnywhere, Category = "Items")
	TArray<TSubclassOf<AItem>> shieldsArray;

	UPROPERTY(EditAnywhere, Category = "Items")
	TArray<TSubclassOf<AItem>> spellsArray;

	UPROPERTY(EditAnywhere, Category = "Items")
	TArray<TSubclassOf<AItem>> weaponsArray;

	UPROPERTY(EditAnywhere, Category = "Items")
	TSubclassOf<AItem> gold;

	UPROPERTY(EditAnywhere, Category = "Items")
	TSubclassOf<AItem> silver;

	UPROPERTY(EditAnywhere, Category = "Items")
	TSubclassOf<AItem> bronze;



	uint8 dropChance;


	uint8 loopTime;
public:	
	UFUNCTION()
	void SpawnItem(FVector Location, FRotator Rotation);

	void GenerateBlacksmithItems(class UShopItems * shopItems, uint8 itemsStrongness);

	void GenerateAlchemistItems(class UShopItems * shopItems, uint8 itemsStrongness);


public:
	// Sets default values for this component's properties
	UItemSpawner();
	
};
