// Fill out your copyright notice in the Description page of Project Settings.

#include "InventorySlot.h"
#include "../Public/AttributeSystem.h"
#include "../Public/EquipmentSystem.h"
#include "../Public/InventorySystem.h"
#include "../Components/ShopSystem.h"
#include "../LoDCharacter.h"
#include "../LoDPlayerController.h"
#include "../Public/References.h"
#include "Input/Reply.h"
#include "Engine.h"



void UInventorySlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("OnDragDetected drag C++!"));
	OutOperation = CreateItemDragDrop();
}

bool UInventorySlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::OnDrop(InGeometry, InDragDropEvent, InOperation);

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("OnDrop drag C++!"));

	UItemDragDropOperation * dragOeration = Cast<UItemDragDropOperation>(InOperation);

	/** Check which slot is drop slot */
	switch (this->slotType)
	{
	case EDragDropSlotType::INVENTORY_SLOT:
		/** If draged item was from inventory, and drop slot is in inventory */
		if (dragOeration->draggedSlot->slotType == EDragDropSlotType::INVENTORY_SLOT)
		{	/** Drop slot contain item, swap items */
			if (this->item)
			{
				/** Create temp variable to store item */
				UInventorySlot * tempItemSlot = NewObject<UInventorySlot>();

				/** Swap items */
				tempItemSlot->item = dragOeration->draggedSlot->item;
				dragOeration->draggedSlot->item = this->item;
				this->item = tempItemSlot->item;

				/** Update image of inventory slot, with item image */
				dragOeration->draggedSlot->itemImage->SetBrushFromTexture(dragOeration->draggedSlot->item->itemTexture);
				this->itemImage->SetBrushFromTexture(this->item->itemTexture);

				if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("INVENTORY Items Swapped!"));

			}
			/** Drop slot is empty, */
			else
			{
				this->item = dragOeration->draggedSlot->item;
				this->itemImage->SetRenderScale(FVector2D(1.0f, 1.0f));
				this->itemImage->SetBrushFromTexture(this->item->itemTexture);

				dragOeration->draggedSlot->item = NULL;
				dragOeration->draggedSlot->itemImage->SetRenderScale(FVector2D(0.0f, 0.0f));

			}
		}
		else if (dragOeration->draggedSlot->slotType == EDragDropSlotType::SHOP_SLOT)
		{

		}
		/** If draged item was from equipment system */
		else
		{
			/** Drop slot contain item, swap items */
			if (this->item && this->item->GetItemPropertyType() == dragOeration->draggedSlot->item->GetItemPropertyType())
			{
				/** Check if player has enough level to wear item */
				if (ptrCharacter->GetAttributeSystem()->GetLevel() >= this->item->GetRequiredLevel())
				{
					{	/** Character wear something, swap equipment. Pass this inventory slot to equipment system.
						Equipment system swap variable pointers */
						ptrCharacter->GetEquipmentSystem()->SwapItemWithInventoryItem(this);
					}
				}
			}
			/** Drop slot is empty, move item */
			else
			{

				this->item = dragOeration->draggedSlot->item;
				this->itemImage->SetRenderScale(FVector2D(1.0f, 1.0f));
				this->itemImage->SetBrushFromTexture(this->item->itemTexture);

				ptrCharacter->GetInventorySystem()->DecreaseCountOfEmptySlotsBy(1);
				ptrCharacter->GetEquipmentSystem()->UnEquipItem(dragOeration->draggedSlot);
			}
		}
		break;

	case EDragDropSlotType::SHOP_SLOT:
		/** If draged item was from inventory */
		if (dragOeration->draggedSlot->slotType == EDragDropSlotType::INVENTORY_SLOT)
		{
			//ptrCharacter->InventorySystem->
		}
		break;

	default: /** Drop slot is in equipment system */
		/** If item is not equipable, get out of this function and do not do anything */
		if (!dragOeration->draggedSlot->item->IsEquipable()) return true;

		/** If draged item was from inventory */
		if (dragOeration->draggedSlot->slotType == EDragDropSlotType::INVENTORY_SLOT)
		{
			/** Check if player has enough level to wear new item */
			if (ptrCharacter->GetAttributeSystem()->GetLevel() >= this->item->GetRequiredLevel())
			{
				/** Character wear something, swap items if items are same properties */
				if (this->item && this->item->GetItemPropertyType() == dragOeration->draggedSlot->item->GetItemPropertyType())
				{
					/**  Pass this inventory slot to equipment system. Equipment system swaps variable pointers */
					ptrCharacter->GetEquipmentSystem()->SwapItemWithInventoryItem(dragOeration->draggedSlot);
				}
				/** Drop slot is empty, equip item if player has enough level */
				else
				{
					this->item = dragOeration->draggedSlot->item;
					this->itemImage->SetRenderScale(FVector2D(1.0f, 1.0f));
					this->itemImage->SetBrushFromTexture(this->item->itemTexture);

					ptrCharacter->GetEquipmentSystem()->EquipItem(this);
					ptrCharacter->GetInventorySystem()->RemoveItemFromSlot(dragOeration->draggedSlot);
				}
			}
		}
		break;
	}
	return true;
}


void UInventorySlot::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) 
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("NativeOnDragCancelled drag C++!"));
}


FReply UInventorySlot::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply reply = Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);
	/** Detect mouse click only if slot contain any item */
	if (item)
	{
		/** Left mouse button is down */
		if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
		{
			return	reply.DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
		}

		/** Right mouse button is down */
		else if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
		{			
			switch (this->slotType)
			{
			case EDragDropSlotType::INVENTORY_SLOT:
				/** If is shop closed, try to equip item */
				if (!ptrPlayerController->shopSystem->IsOpened())
				{
					/** If clicked item is health or mana potion, drink it */
					if (this->item->GetItemPropertyType() == EItemPropertyType::MANA_POTION || this->item->GetItemPropertyType() == EItemPropertyType::HEALTH_POTION)
					{
						/** BLA BLA BLAAAA ----> PUT LOGIC HERE -- DRINK POTION OR SOMETHIING*/
						/** BLA BLA BLAAAA ----> PUT LOGIC HERE -- DRINK POTION OR SOMETHIING*/
						/** BLA BLA BLAAAA ----> PUT LOGIC HERE -- DRINK POTION OR SOMETHIING*/
						/** BLA BLA BLAAAA ----> PUT LOGIC HERE -- DRINK POTION OR SOMETHIING*/
					}
					/** Clicked item is something equipable, check if can player is able to equip item */
					else
					{
						/** Check if player has enough level to wear item */
						if (ptrCharacter->GetAttributeSystem()->GetLevel() >= this->item->GetRequiredLevel())
						{
							if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Going to check if I should equip or swap item!"));

							/** Check if character has equiped anything */
							if (ptrCharacter->GetEquipmentSystem()->HasEquipedItemInSlot(this->item))
							{	/** Character wear something, swap equipment. Pass this inventory slot to equipment system.
								 Equipment system swap variable pointers */
								if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Going to swap item!"));
								ptrCharacter->GetEquipmentSystem()->SwapItemWithInventoryItem(this);
								if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Item Swapped!"));
							}

							else
							{	/** Nothing is equiped, equip this */
								ptrCharacter->GetEquipmentSystem()->EquipItem(this);
								ptrCharacter->GetInventorySystem()->RemoveItemFromSlot(this);
								if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Item Equiped!"));
							}
						}
					}
				}
				// Shop is opened, sell item
				else
				{
					// Before item will get removed from inventory, add him into shop system
					ptrPlayerController->shopSystem->AddItemToShop(this);
					// Sell item, pass pointer of this slot to the inventory system
					ptrCharacter->GetInventorySystem()->SellItem(this);
				}
				break;
			case EDragDropSlotType::SHOP_SLOT:
				/** If player has enough money and space, item will be added into inventory system, 
					and money will be decreased, if not or has not enough space, play audio */
				if (ptrCharacter->GetInventorySystem()->CanBuyItem(this)) ptrPlayerController->shopSystem->RemoveItemFromShop(this);
				break;
			}
		}
	}
	
	return reply;
}


UItemDragDropOperation * UInventorySlot::CreateItemDragDrop() const
{
	UItemDragDropOperation * drag_drop_operation = NewObject<UItemDragDropOperation>();
	drag_drop_operation->draggedSlot = const_cast<UInventorySlot*>(this);
	drag_drop_operation->DefaultDragVisual = this->GetRootWidget();
	drag_drop_operation->Pivot = EDragPivot::MouseDown;
	return drag_drop_operation;
}

void UInventorySlot::AssignImage(UImage * newImage)
{
	this->itemImage = newImage;
}













void UInventorySlot::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("OnDragEnter drag C++!"));
}

void UInventorySlot::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("OnDragLeave drag C++!"));
}

bool UInventorySlot::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("DRAG OVER INDEX: %f"), this->slotIndex));
	return false;
}