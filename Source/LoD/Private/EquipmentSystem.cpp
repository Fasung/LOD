// Fill out your copyright notice in the Description page of Project Settings.
#include "EquipmentSystem.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/GameEngine.h"
#include "References.h"
#include "AttributeSystem.h"
#include "InventorySystem.h"
#include "Item.h"
#include "../LoDEnumTypes.h"
#include "../LoDCharacter.h"
#include "../UserInterface/InventorySlot.h"

// Sets default values for this component's properties
UEquipmentSystem::UEquipmentSystem()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UEquipmentSystem::EquipItem(UInventorySlot * newSlot)
{
	switch (newSlot->GetItem()->GetItemPropertyType())
	{
	case EItemPropertyType::HELMET:
		helmetSlot->SetItem(newSlot->GetItem());
		helmetSlot->ResetRenderScale();
		helmetSlot->SetBrushFromItemTexture();
		helmetSlot->GetItem()->AttachToComponent(characterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, "headSocket");
		break;

	case EItemPropertyType::CHEST:		
		chestSlot->SetItem(newSlot->GetItem());
		chestSlot->ResetRenderScale();
		chestSlot->SetBrushFromItemTexture();
		/** TODO: MISSING attaching mesh to socket */
		break;

	case EItemPropertyType::GLOVES:		
		glovesSlot->SetItem(newSlot->GetItem());
		glovesSlot->ResetRenderScale();
		glovesSlot->SetBrushFromItemTexture();
		/** TODO: MISSING attaching mesh to socket */
		break;

	case EItemPropertyType::PANTS:		
		pantsSlot->SetItem(newSlot->GetItem());
		pantsSlot->ResetRenderScale();
		pantsSlot->SetBrushFromItemTexture();
		/** TODO: MISSING attaching mesh to socket */
		break;

	case EItemPropertyType::BOOTS:
		bootsSlot->SetItem(newSlot->GetItem());
		bootsSlot->ResetRenderScale();
		bootsSlot->SetBrushFromItemTexture();
		bootsSlot->GetItem()->Mesh->AttachToComponent(characterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, "rightFeetSocket");

		bootsSlot->GetItem()->MeshAdditional->AttachToComponent(characterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, "leftFeetSocket");
		bootsSlot->GetItem()->MeshAdditional->SetVisibility(true);

		break;
	case EItemPropertyType::ONE_HAND_WEAPON:
		rightHandSlot->SetItem(newSlot->GetItem());
		rightHandSlot->SetRenderScale(1.85f, 1.15f);
		rightHandSlot->SetBrushFromItemTexture();
		rightHandSlot->GetItem()->AttachToComponent(characterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, "rightHandSocket");
		rightHandSlot->GetItem()->Mesh->CastShadow = true;
		rightHandSlot->GetItem()->Mesh->bCastDynamicShadow = true;
		break;
	case EItemPropertyType::TWO_HAND_WEAPON:		
		/** NOT IMPLEMENTED*/
		/** NOT IMPLEMENTED*/
		/** NOT IMPLEMENTED*/
		/** NOT IMPLEMENTED*/
		/** TODO: MISSING attaching mesh to socket */
		break; 
		
	case EItemPropertyType::SHIELD:
		leftHandSlot->SetItem(newSlot->GetItem());
		leftHandSlot->SetRenderScale(1.5f, 0.75f);
		leftHandSlot->SetBrushFromItemTexture();
		leftHandSlot->GetItem()->AttachToComponent(characterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, "leftHandSocket");
		leftHandSlot->GetItem()->Mesh->CastShadow = true;
		leftHandSlot->GetItem()->Mesh->bCastDynamicShadow = true;
		break;
	case EItemPropertyType::RING:		return;			break;
	default:
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Given invadil item property type, can not equip item.")); return; ;
	}


	// Amend attribute system
	IncreaseAttributes(newSlot->GetItem());


	newSlot->GetItem()->Mesh->SetVisibility(true);

}

void UEquipmentSystem::UnEquipItem(UInventorySlot * oldEquipment)
{
	/** Subtract item atrributes from atribute system */
	DecreaseAttributes(oldEquipment->GetItem());

	/** Detach item from character transform */
	switch (oldEquipment->GetItem()->GetItemPropertyType())
	{
	case EItemPropertyType::HELMET:
		helmetSlot->GetItem()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		break;
	case EItemPropertyType::CHEST:		return;			break;
	case EItemPropertyType::GLOVES:		return;			break;
	case EItemPropertyType::PANTS:		return;			break;
	case EItemPropertyType::BOOTS:
		bootsSlot->GetItem()->Mesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		bootsSlot->GetItem()->MeshAdditional->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		bootsSlot->GetItem()->MeshAdditional->SetVisibility(false);
		break;
	case EItemPropertyType::ONE_HAND_WEAPON:

		rightHandSlot->GetItem()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		rightHandSlot->GetItem()->Mesh->CastShadow = false;
		rightHandSlot->GetItem()->Mesh->bCastDynamicShadow = false;
		break;
	case EItemPropertyType::TWO_HAND_WEAPON:		return;		break;
	case EItemPropertyType::SHIELD:

		leftHandSlot->GetItem()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		leftHandSlot->GetItem()->Mesh->CastShadow = false;
		leftHandSlot->GetItem()->Mesh->bCastDynamicShadow = false;
		break;
	case EItemPropertyType::RING:		return;			break;
	default:
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Given invadil item property type, can not equip item.")); return; ;
	}

	/** Disable visibility and empty current equipment slot */
	oldEquipment->GetItem()->Mesh->SetVisibility(false);
	oldEquipment->SetItemToNull();
	oldEquipment->SetRenderScale(0.0f, 0.0f);
}

const bool UEquipmentSystem::HasEquipedItemInSlot(AItem * newItem)
{
	switch (newItem->GetItemPropertyType())
	{
	case EItemPropertyType::HELMET:
		if (helmetSlot->GetItem()) return true;
		else return false;;
	case EItemPropertyType::CHEST:
		if (chestSlot->GetItem()) return true;
		else return false;
	case EItemPropertyType::GLOVES:
		if (glovesSlot->GetItem()) return true;
		else return false;
	case EItemPropertyType::PANTS:
		if (pantsSlot->GetItem()) return true;
		else return false;
	case EItemPropertyType::BOOTS:
		if (bootsSlot->GetItem()) return true;
		else return false;
	case EItemPropertyType::ONE_HAND_WEAPON:
		if (rightHandSlot->GetItem()) return true;
		else return false;
	case EItemPropertyType::TWO_HAND_WEAPON:
		break;
	case EItemPropertyType::SHIELD:
		if (leftHandSlot->GetItem()) return true;
		else return false;
	case EItemPropertyType::RING:
		if (ringOneSlot->GetItem() && ringTwoSlot->GetItem()) return true;
		else return false;
	default: return false;
	}

	return false;
}

void UEquipmentSystem::SwapItemWithInventoryItem(UInventorySlot * newEquipment)
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Inside swapping function!"));

	/** Create temp variable to store item */
	UInventorySlot * tempItemSlot = NewObject<UInventorySlot>();
	/** Store items */
	tempItemSlot->SetItem(newEquipment->GetItem());
	
	IncreaseAttributes(newEquipment->GetItem());

	/** Swap Items */
	switch (newEquipment->GetItem()->GetItemPropertyType())
	{
	case EItemPropertyType::HELMET:
		/** Amend attribute system before starting swapping*/
		DecreaseAttributes(this->helmetSlot->GetItem());
		/** Swap items*/
		newEquipment->SetItem(this->helmetSlot->GetItem());
		this->helmetSlot->SetItem(tempItemSlot->GetItem());
		newEquipment->SetBrushFromItemTexture();
		this->helmetSlot->SetBrushFromItemTexture();
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Swapping helmets !!!!"));
		return;

	case EItemPropertyType::CHEST:
		/** Amend attribute system before starting swapping*/
		DecreaseAttributes(this->chestSlot->GetItem());
		/** Swap items*/
		newEquipment->SetItem(this->chestSlot->GetItem());
		this->chestSlot->SetItem(tempItemSlot->GetItem());
		newEquipment->SetBrushFromItemTexture();
		this->chestSlot->SetBrushFromItemTexture();
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Swapping chests !!!!"));
		return;

	case EItemPropertyType::GLOVES:
		/** Amend attribute system before starting swapping*/
		DecreaseAttributes(this->glovesSlot->GetItem());
		/** Swap items*/
		newEquipment->SetItem(this->glovesSlot->GetItem());
		this->glovesSlot->SetItem(tempItemSlot->GetItem());
		newEquipment->SetBrushFromItemTexture();
		this->glovesSlot->SetBrushFromItemTexture();
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Swapping gloves !!!!"));
		return;

	case EItemPropertyType::PANTS:
		/** Amend attribute system before starting swapping*/
		DecreaseAttributes(this->pantsSlot->GetItem());
		/** Swap items*/
		newEquipment->SetItem(this->pantsSlot->GetItem());
		this->pantsSlot->SetItem(tempItemSlot->GetItem());
		newEquipment->SetBrushFromItemTexture();
		this->pantsSlot->SetBrushFromItemTexture();
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Swapping pants !!!!"));
		return;

	case EItemPropertyType::BOOTS:
		/** Amend attribute system before starting swapping*/
		DecreaseAttributes(this->bootsSlot->GetItem());
		/** Swap items*/
		newEquipment->SetItem(this->bootsSlot->GetItem());
		this->bootsSlot->SetItem(tempItemSlot->GetItem());
		newEquipment->SetBrushFromItemTexture();
		this->bootsSlot->SetBrushFromItemTexture();
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Swapping boots !!!!"));
		return;

	case EItemPropertyType::ONE_HAND_WEAPON:
		/** Amend attribute system before starting swapping*/
		DecreaseAttributes(this->rightHandSlot->GetItem());

		this->rightHandSlot->GetItem()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		this->rightHandSlot->GetItem()->Mesh->SetVisibility(false);

		this->rightHandSlot->GetItem()->Mesh->CastShadow = false;
		this->rightHandSlot->GetItem()->Mesh->bCastDynamicShadow = false;

		/** Swap items*/
		newEquipment->SetItem(this->rightHandSlot->GetItem());
		this->rightHandSlot->SetItem(tempItemSlot->GetItem());
		newEquipment->SetBrushFromItemTexture();
		this->rightHandSlot->SetBrushFromItemTexture();

		rightHandSlot->GetItem()->AttachToComponent(characterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, "rightHandSocket");
		this->rightHandSlot->GetItem()->Mesh->SetVisibility(true);

		this->rightHandSlot->GetItem()->Mesh->CastShadow = true;
		this->rightHandSlot->GetItem()->Mesh->bCastDynamicShadow = true;

		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Swapping one hand weapons !!!!"));
		return;

	case EItemPropertyType::TWO_HAND_WEAPON:
		/* NOT IMPLEMENTED */
		/* NOT IMPLEMENTED */
		/* NOT IMPLEMENTED */
		/* NOT IMPLEMENTED */
		/* NOT IMPLEMENTED */
		/* NOT IMPLEMENTED */
		return;

	case EItemPropertyType::SHIELD:
		/** Amend attribute system before starting swapping*/
		DecreaseAttributes(this->leftHandSlot->GetItem());

		this->leftHandSlot->GetItem()->Mesh->CastShadow = false;
		this->leftHandSlot->GetItem()->Mesh->bCastDynamicShadow = false;

		this->leftHandSlot->GetItem()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		this->leftHandSlot->GetItem()->Mesh->SetVisibility(false);

		/** Swap items*/
		newEquipment->SetItem(this->leftHandSlot->GetItem());
		this->leftHandSlot->SetItem(tempItemSlot->GetItem());
		newEquipment->SetBrushFromItemTexture();
		this->leftHandSlot->SetBrushFromItemTexture();

		leftHandSlot->GetItem()->AttachToComponent(characterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, "leftHandSocket");
		this->leftHandSlot->GetItem()->Mesh->SetVisibility(true);

		this->leftHandSlot->GetItem()->Mesh->CastShadow = true;
		this->leftHandSlot->GetItem()->Mesh->bCastDynamicShadow = true;

		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Swapping shields !!!!"));
		return;

	case EItemPropertyType::RING:
		/* NOT IMPLEMENTED */
		/* NOT IMPLEMENTED */
		/* NOT IMPLEMENTED */
		/* NOT IMPLEMENTED */
		/* NOT IMPLEMENTED */
		/* NOT IMPLEMENTED */
		return;
	}

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Reach the end of the swapping functions inside equipments system, THIS IS BUG !!!!"));
}


void UEquipmentSystem::IncreaseAttributes(AItem * item)
{
	this->attributeSystem->strength += item->GetStrenght();
	this->attributeSystem->agility += item->GetAgility();
	this->attributeSystem->vitality += item->GetVitality();
	this->attributeSystem->intelligence += item->GetIntelligence();
	this->attributeSystem->spirit += item->GetSpirit();
	this->attributeSystem->armor += item->GetArmor();

	this->attributeSystem->magicResistance += item->GetMagicResistance();
	this->attributeSystem->minimumAttackPowerFromWeapon += item->GetMinimalDamge();
	this->attributeSystem->maximumAttackPowerFromWeapon += item->GetMaximalDamage();
}

void UEquipmentSystem::DecreaseAttributes(AItem * item)
{
	this->attributeSystem->strength -= item->GetStrenght();
	this->attributeSystem->agility -= item->GetAgility();
	this->attributeSystem->vitality -= item->GetVitality();
	this->attributeSystem->intelligence -= item->GetIntelligence();
	this->attributeSystem->spirit -= item->GetSpirit();
	this->attributeSystem->armor -= item->GetArmor();

	this->attributeSystem->magicResistance -= item->GetMagicResistance();
	this->attributeSystem->minimumAttackPowerFromWeapon -= item->GetMinimalDamge();
	this->attributeSystem->maximumAttackPowerFromWeapon -= item->GetMaximalDamage();
}

void UEquipmentSystem::StartDrinkPotion(UInventorySlot * newSlot)
{
	this->inventorySlotWhichContaintPotion = newSlot;
	this->inventorySlotWhichContaintPotion->GetItem()->Mesh->SetVisibility(true);
	this->inventorySlotWhichContaintPotion->GetItem()->AttachToComponent(characterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, "potionSocket");

	if (rightHandSlot->GetItem()) rightHandSlot->GetItem()->Mesh->SetVisibility(false);
}

void UEquipmentSystem::StopDrinkingPotion()
{
	if (rightHandSlot->GetItem()) rightHandSlot->GetItem()->Mesh->SetVisibility(true);

	if (this->inventorySlotWhichContaintPotion->GetItem())
	{
		this->inventorySlotWhichContaintPotion->GetItem()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		
		ptrCharacter->GetInventorySystem()->DestroyItemFromSlot(this->inventorySlotWhichContaintPotion);
		this->inventorySlotWhichContaintPotion = NULL;
	}
}
