// Fill out your copyright notice in the Description page of Project Settings.

#include "InventorySystem.h"
#include "TargetingSystem.h"
#include "../UserInterface/InventorySlot.h"
#include "Blueprint/UserWidget.h"
#include "Button.h"
#include "References.h"
#include "Item.h"
#include "Engine.h"
#include "../LoDCharacter.h"


// Sets default values for this component's properties
UInventorySystem::UInventorySystem() :
	emptySlots(0),
	totalSlotsCount(0),
	golds(0),
	silvers(0),
	bIsOpened(0),
	countOfHealthPotions(0),
	countOfManaPotions(0)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void UInventorySystem::BeginPlay()
{
	Super::BeginPlay();

	BTN_CloseInventory->OnClicked.AddDynamic(this, &UInventorySystem::OnClickedCloseInventoryButton);
}

bool UInventorySystem::PickUpItem(AItem * newItem)
{
	/** If the item is Gold or Silver type */
	switch (newItem->GetItemPropertyType())
	{
	case EItemPropertyType::MONEY_GOLD:
		/** Addjust golds */
		golds += newItem->GetGoldsValue();
		// Clear the targeting system
		ptrCharacter->GetTargetingSystem()->ClearTarget();
		// try and play the sound if specified
		if (newItem->AC_PickUp)	UGameplayStatics::PlaySoundAtLocation(this, newItem->AC_PickUp, newItem->GetActorLocation());
		/** Destroy object */
		newItem->Destroy();
		return false;

	case EItemPropertyType::MONEY_SILVER:
		/** Addjust silvers */
		silvers += newItem->GetSilversValue();
		/** Check if silvers are not higher than 99, if yes addjust golds and reset silver */
		if (silvers > 99) { silvers -= 100; golds++; }

		/** Clear the targeting system */
		ptrCharacter->GetTargetingSystem()->ClearTarget();
		/** try and play the sound if specified */
		if (newItem->AC_PickUp)	UGameplayStatics::PlaySoundAtLocation(this, newItem->AC_PickUp, newItem->GetActorLocation());
		/** Destroy object */
		newItem->Destroy();
		return false;

	case EItemPropertyType::MONEY_BRONZE:
		/** Clear the targeting system */
		ptrCharacter->GetTargetingSystem()->ClearTarget();
		/** try and play the sound if specified */
		if (newItem->AC_PickUp)	UGameplayStatics::PlaySoundAtLocation(this, newItem->AC_PickUp, newItem->GetActorLocation());
		/** Destroy object */
		newItem->Destroy();
		return false;
	}

	if (emptySlots)
	{
		// Find the empty slot in inventory, example if inside inventory is 10 items, and the second slot in inventory is empty, assign second slot to this item
		for (int i = 0; i < totalSlotsCount; i++)
		{
			if (!this->inventorySlots[i]->GetItem())
			{
				/** It is very important to set new item first, because function SetBrushFromTexture use texture which containt item */
				this->inventorySlots[i]->SetItem(newItem);
				this->inventorySlots[i]->ResetRenderScale();
				this->inventorySlots[i]->SetBrushFromItemTexture();

				switch (newItem->GetItemPropertyType())
				{
				case EItemPropertyType::HEALTH_POTION:
					countOfHealthPotions++;
					break;
				case EItemPropertyType::MANA_POTION:
					countOfManaPotions++;
					break;
				}

				// Descrease empty slots
				emptySlots--;
				return true;
			}
		}
	}

	// TODO: PLAY AUDIO THAT INVENTORY IS FULL, OR SOMETHING ELSE
	return false;
	
}

/** Throw away item from inventory array */
void UInventorySystem::ThrowItem(AItem * newItem)
{
	emptySlots++;
}

void UInventorySystem::RemoveItemFromSlot(UInventorySlot * itemSlot)
{
	
	this->inventorySlots[itemSlot->slotIndex]->SetItem(NULL);
	this->inventorySlots[itemSlot->slotIndex]->SetRenderScale(0.0f, 0.0f);
	
	emptySlots++;
}

void UInventorySystem::DestroyItemFromSlot(class UInventorySlot * itemSlot)
{
	this->inventorySlots[itemSlot->slotIndex]->GetItem()->Destroy();
	this->inventorySlots[itemSlot->slotIndex]->SetItem(NULL);
	this->inventorySlots[itemSlot->slotIndex]->SetRenderScale(0.0f, 0.0f);

	emptySlots++;
}


void UInventorySystem::SellItem(UInventorySlot * itemSlot)
{
	this->golds += itemSlot->GetItem()->GetGoldsValue();
	this->silvers += itemSlot->GetItem()->GetSilversValue();

	if (silvers > 99) { silvers -= 100; golds++; }

	switch (itemSlot->GetItem()->GetItemPropertyType())
	{
	case EItemPropertyType::HEALTH_POTION:
		countOfHealthPotions--;
		break;
	case EItemPropertyType::MANA_POTION:
		countOfManaPotions--;
		break;
	}

	this->inventorySlots[itemSlot->slotIndex]->SetItem(NULL);
	this->inventorySlots[itemSlot->slotIndex]->SetRenderScale(0.0f, 0.0f);

	emptySlots++;
}


bool UInventorySystem::CanBuyItem(UInventorySlot * shopSlot)
{
	/** If has space in inventory, try to buy item */
	if (emptySlots)
	{
		/** Check if player has enough money to buy item */
		//if (this->golds)
		{
			// Find the empty slot in inventory, example if inside inventory is 10 items, and the second slot in inventory is empty, assign second slot to this item
			for (int i = 0; i < totalSlotsCount; i++)
			{
				if (!this->inventorySlots[i]->GetItem())
				{
					/** It is very important to set new item first, because function SetBrushFromTexture use texture which containt item */
					this->inventorySlots[i]->SetItem(shopSlot->GetItem());
					this->inventorySlots[i]->ResetRenderScale();
					this->inventorySlots[i]->SetBrushFromItemTexture();

					switch (shopSlot->GetItem()->GetItemPropertyType())
					{
					case EItemPropertyType::HEALTH_POTION:
						countOfHealthPotions++;
						break;
					case EItemPropertyType::MANA_POTION:
						countOfManaPotions++;
						break;
					}

					// Descrease empty slots
					emptySlots--;
					return true;
				}
			}
		}
		// Play audio not enough gold
	}
	// Play audio not enough space
	//else 
	
	return false;
}

/** Assign new inventory widget */
void UInventorySystem::AssignInventoryWidget(UUserWidget * newInventoryWidget) 
{
	this->UW_Inventory = newInventoryWidget; 
	this->UW_Inventory->SetVisibility(ESlateVisibility::Hidden);
}

/** Assign new inventory slots widgets */
void UInventorySystem::AssignInventorySlots(TArray<UInventorySlot*> newInventorySlots)
{
	this->inventorySlots = newInventorySlots;
	this->totalSlotsCount = newInventorySlots.Num();
	this->emptySlots = this->totalSlotsCount;

	/** Assign slots indicies */
	for (uint8 i = 0; i < totalSlotsCount; i++)	inventorySlots[i]->slotIndex = i;
}

/** Assign close inventory button into C++ button pointer */
void UInventorySystem::AssignCloseInventoryButton(UButton * newButton)
{
	this->BTN_CloseInventory = newButton; 
}

/** On left mouse button clicked onto inventory button, close inventory */
void UInventorySystem::OnClickedCloseInventoryButton()
{
	this->bIsOpened = false; 
	this->UW_Inventory->SetVisibility(ESlateVisibility::Hidden); 
}

/** Set inventory widget visibility to hidden */
void UInventorySystem::CloseInventory() 
{
	this->UW_Inventory->SetVisibility(ESlateVisibility::Hidden); 
	this->bIsOpened = false;
}

/** Set inventory widget visibility to visible */
void UInventorySystem::OpenInventory() 
{
	this->UW_Inventory->SetVisibility(ESlateVisibility::Visible);
	this->bIsOpened = true; 
}

UInventorySlot * UInventorySystem::GetHealthPotion() const
{
	for (int i = 0; i < totalSlotsCount - emptySlots; i++)
	{
		if (this->inventorySlots[i]->GetItem()->GetItemPropertyType() == EItemPropertyType::HEALTH_POTION) return this->inventorySlots[i];
	}
	
	return false;
}

UInventorySlot * UInventorySystem::GetManaPotion() const
{
	for (int i = 0; i < totalSlotsCount - emptySlots; i++)
	{
		if (this->inventorySlots[i]->GetItem()->GetItemPropertyType() == EItemPropertyType::MANA_POTION) return this->inventorySlots[i];
	}

	return false;
}