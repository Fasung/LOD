// Fill out your copyright notice in the Description page of Project Settings.

#include "NPC_Shop.h"
#include "../Public/ItemSpawner.h"
#include "References.h"
#include "../Components/ShopSystem.h"
#include "../LoDGameMode.h"

ANPC_Shop::ANPC_Shop()
{
	shopItems = CreateDefaultSubobject<UShopItems>(TEXT("ShopItems"));
}


void ANPC_Shop::BeginPlay()
{
	Super::BeginPlay();
	

	switch (shopType)
	{
	case EShopType::BLACKSMITH: ptrGameMode->itemSpawner->GenerateBlacksmithItems(this->shopItems, this->itemsStrongness);
		break;
	case EShopType::ALCHEMIST: ptrGameMode->itemSpawner->GenerateAlchemistItems(this->shopItems, this->itemsStrongness);
		break;
	}
}
