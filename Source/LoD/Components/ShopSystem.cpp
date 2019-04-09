// Fill out your copyright notice in the Description page of Project Settings.

#include "ShopSystem.h"
#include "Blueprint/UserWidget.h"
#include "TextBlock.h"
#include "Button.h"
#include "../Public/Item.h"
#include "../UserInterface/InventorySlot.h"
#include "VerticalBox.h"
#include "ItemSpawner.h"
#include "Image.h"
#include "Engine.h"

#define NOT_SELECTED_BUTTON_FONT_SIZE 15
#define SELECTED_BUTTON_FONT_SIZE 20

// Sets default values for this component's properties
UShopSystem::UShopSystem()	:
	bIsOpenedArmorsSlot(0),
	bIsOpenedJewelrySlot(0),
	bIsOpenedShieldsSlot(0),
	bIsOpenedWeaponsSlot(0),
	bIsOpenedPotionsSlot(0),
	bIsOpenedSpellsSlot(0)
{
	this->BS_ButtonNotSelected.Normal.TintColor = FSlateColor(FLinearColor(0.20451f, 0.0f, 0.028216, 1.0f));
	this->BS_ButtonNotSelected.Hovered.TintColor = FSlateColor(FLinearColor(0.30451f, 0.0f, 0.028216, 1.0f));
	this->BS_ButtonNotSelected.Pressed.TintColor = FSlateColor(FLinearColor(0.20451f, 0.0f, 0.028216, 1.0f));

	this->BS_ButtonNotSelected.Normal.DrawAs = ESlateBrushDrawType::Box;
	this->BS_ButtonNotSelected.Hovered.DrawAs = ESlateBrushDrawType::Box;
	this->BS_ButtonNotSelected.Pressed.DrawAs = ESlateBrushDrawType::Box;

	this->BS_ButtonNotSelected.NormalPadding.Bottom = 3.0f;
	this->BS_ButtonNotSelected.NormalPadding.Top = 3.0f;
	this->BS_ButtonNotSelected.NormalPadding.Left = 3.0f;
	this->BS_ButtonNotSelected.NormalPadding.Right = 3.0f;

	this->BS_ButtonNotSelected.PressedPadding.Bottom = 1.5f;
	this->BS_ButtonNotSelected.PressedPadding.Top = 4.5f;
	this->BS_ButtonNotSelected.PressedPadding.Left = 3.0f;
	this->BS_ButtonNotSelected.PressedPadding.Right = 3.0f;

	this->BS_ButtonSelected.Normal.TintColor = FSlateColor(FLinearColor(0.42396f, 0.301978f, 0.011209f, 1.0f));
	this->BS_ButtonSelected.Hovered.TintColor = FSlateColor(FLinearColor(0.42396f, 0.301978f, 0.011209f, 1.0f));
	this->BS_ButtonSelected.Pressed.TintColor = FSlateColor(FLinearColor(0.42396f, 0.301978f, 0.011209f, 1.0f));

	this->BS_ButtonSelected.Normal.DrawAs = ESlateBrushDrawType::Box;
	this->BS_ButtonSelected.Hovered.DrawAs = ESlateBrushDrawType::Box;
	this->BS_ButtonSelected.Pressed.DrawAs = ESlateBrushDrawType::Box;

	this->BS_ButtonSelected.NormalPadding.Bottom = 7.0f;
	this->BS_ButtonSelected.NormalPadding.Top = 7.0f;
	this->BS_ButtonSelected.NormalPadding.Left = 7.0f;
	this->BS_ButtonSelected.NormalPadding.Right = 7.0f;

	this->BS_ButtonSelected.PressedPadding.Bottom = 7.0f;
	this->BS_ButtonSelected.PressedPadding.Top = 7.0f;
	this->BS_ButtonSelected.PressedPadding.Left = 7.0f;
	this->BS_ButtonSelected.PressedPadding.Right = 7.0f;

	this->lastSelectedButton = EShopLastSelectedButton::NONE;

	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bAllowTickOnDedicatedServer = false;
}

// Called when the game starts
void UShopSystem::BeginPlay()
{
	Super::BeginPlay();

	// Bind events to buttons
	BTN_CloseShop->OnClicked.AddDynamic(this, &UShopSystem::OnClickedCloseShopButton);
	BTN_CloseShopX->OnClicked.AddDynamic(this, &UShopSystem::OnClickedCloseShopButton);
	BTN_Armor->OnClicked.AddDynamic(this, &UShopSystem::OnClickedArmorsButton);
	BTN_Jewelry->OnClicked.AddDynamic(this, &UShopSystem::OnClickedJewelryButton);
	BTN_Shields->OnClicked.AddDynamic(this, &UShopSystem::OnClickedShieldsButton);
	BTN_Weapons->OnClicked.AddDynamic(this, &UShopSystem::OnClickedWeaponsButton);
	BTN_Potions->OnClicked.AddDynamic(this, &UShopSystem::OnClickedPotionsButton);
	BTN_Spells->OnClicked.AddDynamic(this, &UShopSystem::OnClickedSpellsButton);
	BTN_Previous->OnClicked.AddDynamic(this, &UShopSystem::OnClickedPreviousButton);
	BTN_Next->OnClicked.AddDynamic(this, &UShopSystem::OnClickedNextButton);


	BTN_Armor->WidgetStyle = BS_ButtonNotSelected;
	BTN_TXT_Armor->SetFont(SFI_ButtonNotSelected);
	BTN_TXT_Armor->SetColorAndOpacity(SC_ButtonNotSelected);

	BTN_Jewelry->WidgetStyle = BS_ButtonNotSelected;
	BTN_TXT_Jewelry->SetFont(SFI_ButtonNotSelected);
	BTN_TXT_Jewelry->SetColorAndOpacity(SC_ButtonNotSelected);

	BTN_Potions->WidgetStyle = BS_ButtonNotSelected;
	BTN_TXT_Potions->SetFont(SFI_ButtonNotSelected);
	BTN_TXT_Potions->SetColorAndOpacity(SC_ButtonNotSelected);
	
	BTN_Shields->WidgetStyle = BS_ButtonNotSelected;
	BTN_TXT_Shields->SetFont(SFI_ButtonNotSelected);
	BTN_TXT_Shields->SetColorAndOpacity(SC_ButtonNotSelected);

	BTN_Spells->WidgetStyle = BS_ButtonNotSelected;
	BTN_TXT_Spells->SetFont(SFI_ButtonNotSelected);
	BTN_TXT_Spells->SetColorAndOpacity(SC_ButtonNotSelected);

	BTN_Weapons->WidgetStyle = BS_ButtonNotSelected;
	BTN_TXT_Weapons->SetFont(SFI_ButtonNotSelected);
	BTN_TXT_Weapons->SetColorAndOpacity(SC_ButtonNotSelected);
}

/** Assign items from armor array to inventory widget */
void UShopSystem::ApplyArmorThread()
{
	/** Clear all shop slots */
	for (uint8 i = 0; i < this->shopSlots.Num(); i++)
	{
		this->shopSlots[i]->SetItem(NULL);
		this->shopSlots[i]->SetRenderScale(0.0f, 0.0f);
	}
	
	if (this->shopItems->bHasArmors)
	{
		for (uint8 i = 0; i < this->shopItems->itemsArmors.Num(); i++)
		{
			this->shopSlots[i]->SetItem(this->shopItems->itemsArmors[i]);
			this->shopSlots[i]->SetBrushFromItemTexture();
			this->shopSlots[i]->ResetRenderScale();
		}
	}

	if (lastSelectedButton != EShopLastSelectedButton::ARMORS)
	{
		BTN_Armor->WidgetStyle = BS_ButtonSelected;
		BTN_TXT_Armor->SetFont(SFI_ButtonSelected);
		BTN_TXT_Armor->SetColorAndOpacity(SC_ButtonSelected);

		lastSelectedButton = EShopLastSelectedButton::ARMORS;

		bIsOpenedArmorsSlot = true;
		bIsOpenedJewelrySlot = false;
		bIsOpenedShieldsSlot = false;
		bIsOpenedWeaponsSlot = false;
		bIsOpenedPotionsSlot = false;
		bIsOpenedSpellsSlot = false;
	}
}

/** Assign items from jewerly array to inventory widget */
void UShopSystem::ApplyJewelryThread()
{
	/** Clear all shop slots */
	for (uint8 i = 0; i < shopSlots.Num(); i++)
	{
		this->shopSlots[i]->SetItem(NULL);
		this->shopSlots[i]->SetRenderScale(0.0f, 0.0f);
	}

	if (this->shopItems->bHasJewelry)
	{
		for (uint8 i = 0; i < this->shopItems->itemsJewelry.Num(); i++)
		{
			this->shopSlots[i]->SetItem(this->shopItems->itemsJewelry[i]);
			this->shopSlots[i]->SetBrushFromItemTexture();
			this->shopSlots[i]->ResetRenderScale();
		}
	}

	if (lastSelectedButton != EShopLastSelectedButton::JEWELRY)
	{
		BTN_Jewelry->WidgetStyle = BS_ButtonSelected;
		BTN_TXT_Jewelry->SetFont(SFI_ButtonSelected);
		BTN_TXT_Jewelry->SetColorAndOpacity(SC_ButtonSelected);

		lastSelectedButton = EShopLastSelectedButton::JEWELRY;

		bIsOpenedArmorsSlot = false;
		bIsOpenedJewelrySlot = true;
		bIsOpenedShieldsSlot = false;
		bIsOpenedWeaponsSlot = false;
		bIsOpenedPotionsSlot = false;
		bIsOpenedSpellsSlot = false;
	}
}

/** Assign items from shields array to inventory widget */
void UShopSystem::ApplyShieldsThread()
{
	/** Clear all shop slots */
	for (uint8 i = 0; i < shopSlots.Num(); i++)
	{
		this->shopSlots[i]->SetItem(NULL);
		this->shopSlots[i]->SetRenderScale(0.0f, 0.0f);
	}
	
	if (this->shopItems->bHasShields)
	{
		for (uint8 i = 0; i < this->shopItems->itemsShields.Num(); i++)
		{
			this->shopSlots[i]->SetItem(this->shopItems->itemsShields[i]);
			this->shopSlots[i]->SetBrushFromItemTexture();
			this->shopSlots[i]->ResetRenderScale();
		}
	}

	if (lastSelectedButton != EShopLastSelectedButton::SHIELDS)
	{
		BTN_Shields->WidgetStyle = BS_ButtonSelected;
		BTN_TXT_Shields->SetFont(SFI_ButtonSelected);
		BTN_TXT_Shields->SetColorAndOpacity(SC_ButtonSelected);

		lastSelectedButton = EShopLastSelectedButton::SHIELDS;

		bIsOpenedArmorsSlot = false;
		bIsOpenedJewelrySlot = false;
		bIsOpenedShieldsSlot = true;
		bIsOpenedWeaponsSlot = false;
		bIsOpenedPotionsSlot = false;
		bIsOpenedSpellsSlot = false;
	}
}

/** Assign items from weapons array to inventory widget */
void UShopSystem::ApplyWeaponsThread()
{
	/** Clear all shop slots */
	for (uint8 i = 0; i < shopSlots.Num(); i++)
	{
		this->shopSlots[i]->SetItem(NULL);
		this->shopSlots[i]->SetRenderScale(0.0f, 0.0f);
	}

	if (this->shopItems->bHasWeapons)
	{
		for (uint8 i = 0; i < this->shopItems->itemsWeapons.Num(); i++)
		{
			this->shopSlots[i]->SetItem(this->shopItems->itemsWeapons[i]);
			this->shopSlots[i]->SetBrushFromItemTexture();
			this->shopSlots[i]->ResetRenderScale();
		}
	}

	if (lastSelectedButton != EShopLastSelectedButton::WEAPONS)
	{
		BTN_Weapons->WidgetStyle = BS_ButtonSelected;
		BTN_TXT_Weapons->SetFont(SFI_ButtonSelected);
		BTN_TXT_Weapons->SetColorAndOpacity(SC_ButtonSelected);

		lastSelectedButton = EShopLastSelectedButton::WEAPONS;
		bIsOpenedArmorsSlot = false;
		bIsOpenedJewelrySlot = false;
		bIsOpenedShieldsSlot = false;
		bIsOpenedWeaponsSlot = true;
		bIsOpenedPotionsSlot = false;
		bIsOpenedSpellsSlot = false;
	}
}

/** Assign items from potions array to inventory widget */
void UShopSystem::ApplyPotionsThread()
{
	 /** Clear all shop slots */
	for (uint8 i = 0; i < shopSlots.Num(); i++)
	{
		this->shopSlots[i]->SetItem(NULL);
		this->shopSlots[i]->SetRenderScale(0.0f, 0.0f);
	}

	if (this->shopItems->bHasPotions)
	{
		for (uint8 i = 0; i < this->shopItems->itemsPotions.Num(); i++)
		{
			this->shopSlots[i]->SetItem(this->shopItems->itemsPotions[i]);
			this->shopSlots[i]->SetBrushFromItemTexture();
			this->shopSlots[i]->ResetRenderScale();
		}
	}

	if (lastSelectedButton != EShopLastSelectedButton::POTIONS)
	{
		BTN_Potions->WidgetStyle = BS_ButtonSelected;
		BTN_TXT_Potions->SetFont(SFI_ButtonSelected);
		BTN_TXT_Potions->SetColorAndOpacity(SC_ButtonSelected);

		lastSelectedButton = EShopLastSelectedButton::POTIONS;

		bIsOpenedArmorsSlot = false;
		bIsOpenedJewelrySlot = false;
		bIsOpenedShieldsSlot = false;
		bIsOpenedWeaponsSlot = false;
		bIsOpenedPotionsSlot = true;
		bIsOpenedSpellsSlot = false;
	}
}

/** Assign items from spells array to inventory widget */
void UShopSystem::ApplySpellsThread()
{
	/** Clear all shop slots */
	for (uint8 i = 0; i < shopSlots.Num(); i++)
	{
		this->shopSlots[i]->SetItem(NULL);
		this->shopSlots[i]->SetRenderScale(0.0f, 0.0f);
	}

	if (this->shopItems->bHasSpells)
	{
		for (uint8 i = 0; i < this->shopItems->itemsSpells.Num(); i++)
		{
			this->shopSlots[i]->SetItem(this->shopItems->itemsSpells[i]);
			this->shopSlots[i]->SetBrushFromItemTexture();
			this->shopSlots[i]->ResetRenderScale();
		}
	}


	if (lastSelectedButton != EShopLastSelectedButton::SPELLS)

	{
		BTN_Spells->WidgetStyle = BS_ButtonSelected;
		BTN_TXT_Spells->SetFont(SFI_ButtonSelected);
		BTN_TXT_Spells->SetColorAndOpacity(SC_ButtonSelected);

		lastSelectedButton = EShopLastSelectedButton::SPELLS;

		bIsOpenedArmorsSlot = false;
		bIsOpenedJewelrySlot = false;
		bIsOpenedShieldsSlot = false;
		bIsOpenedWeaponsSlot = false;
		bIsOpenedPotionsSlot = false;
		bIsOpenedSpellsSlot = true;
	}
}

void UShopSystem::ApplyFirstAvailableShopThread()
{
	UpdateStyleOfLastSelectedButton();
	lastSelectedButton = EShopLastSelectedButton::NONE;

	if (this->shopItems->bHasArmors) ApplyArmorThread();
	else if (this->shopItems->bHasShields) ApplyShieldsThread();
	else if (this->shopItems->bHasWeapons) ApplyWeaponsThread();
	else if (this->shopItems->bHasPotions) ApplyPotionsThread();
	else if (this->shopItems->bHasSpells) ApplySpellsThread();
}

void UShopSystem::RemoveItemFromShop(UInventorySlot * shopSlot)
{
	if (bIsOpenedArmorsSlot)
	{
		shopItems->itemsArmors.Remove(shopSlot->GetItem());
		ApplyArmorThread();
	}
	else if (bIsOpenedJewelrySlot)
	{
		shopItems->itemsJewelry.Remove(shopSlot->GetItem());
		ApplyJewelryThread();
	}
	else if (bIsOpenedShieldsSlot)
	{
		shopItems->itemsShields.Remove(shopSlot->GetItem());
		ApplyShieldsThread();
	}
	else if (bIsOpenedWeaponsSlot)
	{
		shopItems->itemsWeapons.Remove(shopSlot->GetItem());
		ApplyWeaponsThread();
	}
	else if (bIsOpenedPotionsSlot)
	{
		shopItems->itemsPotions.Remove(shopSlot->GetItem());
		ApplyPotionsThread();
	}
	else if (bIsOpenedSpellsSlot)
	{
		shopItems->itemsSpells.Remove(shopSlot->GetItem());
		ApplySpellsThread();
	}
}

void UShopSystem::AddItemToShop(UInventorySlot * sellingItemSlot)
{

	switch (sellingItemSlot->GetItem()->GetItemPropertyType())
	{
	case EItemPropertyType::HELMET:				shopItems->itemsArmors.Add(sellingItemSlot->GetItem());  this->shopItems->bHasArmors = true;	break;
	case EItemPropertyType::CHEST:				shopItems->itemsArmors.Add(sellingItemSlot->GetItem());	 this->shopItems->bHasArmors = true;	break;
	case EItemPropertyType::GLOVES:				shopItems->itemsArmors.Add(sellingItemSlot->GetItem());	 this->shopItems->bHasArmors = true;	break;
	case EItemPropertyType::PANTS:				shopItems->itemsArmors.Add(sellingItemSlot->GetItem());	 this->shopItems->bHasArmors = true;	break;
	case EItemPropertyType::BOOTS:				shopItems->itemsArmors.Add(sellingItemSlot->GetItem());	 this->shopItems->bHasArmors = true;	break;
	case EItemPropertyType::ONE_HAND_WEAPON:	shopItems->itemsWeapons.Add(sellingItemSlot->GetItem()); this->shopItems->bHasWeapons = true;	break;
	case EItemPropertyType::TWO_HAND_WEAPON:	shopItems->itemsWeapons.Add(sellingItemSlot->GetItem()); this->shopItems->bHasWeapons = true;	break;
	case EItemPropertyType::SHIELD:				shopItems->itemsShields.Add(sellingItemSlot->GetItem()); this->shopItems->bHasShields = true;	break;
	case EItemPropertyType::RING: 				shopItems->itemsJewelry.Add(sellingItemSlot->GetItem()); this->shopItems->bHasJewelry = true;	break;
	case EItemPropertyType::NECKLACE:			shopItems->itemsJewelry.Add(sellingItemSlot->GetItem()); this->shopItems->bHasJewelry = true;	break;
	case EItemPropertyType::MANA_POTION:		shopItems->itemsPotions.Add(sellingItemSlot->GetItem()); this->shopItems->bHasPotions = true;	break;
	case EItemPropertyType::HEALTH_POTION:		shopItems->itemsPotions.Add(sellingItemSlot->GetItem()); this->shopItems->bHasPotions = true;	break;
	}

	if (bIsOpenedArmorsSlot) ApplyArmorThread();
	else if (bIsOpenedJewelrySlot) ApplyJewelryThread();
	else if (bIsOpenedShieldsSlot) ApplyShieldsThread();
	else if (bIsOpenedWeaponsSlot) ApplyWeaponsThread();
	else if (bIsOpenedPotionsSlot) ApplyPotionsThread();
	else if (bIsOpenedSpellsSlot) ApplySpellsThread();
}

/** Assign new shop widget */
void UShopSystem::AssignShopWidget(UUserWidget * newInventoryWidget, UImage * imageNPC, UVerticalBox * verticalBox)
{
	this->UW_Shop = newInventoryWidget;
	this->UW_Shop->SetVisibility(ESlateVisibility::Hidden);
	this->VB_ShopContent = verticalBox;
	this->IMAGE_NPC = imageNPC;
}

/** Assign new shop slots widgets */
void UShopSystem::AssignShopSlots(TArray<UInventorySlot*> newInventorySlots)
{
	this->shopSlots = newInventorySlots;

	/** Assign slots indicies */
	for (uint8 i = 0; i < this->shopSlots.Num(); i++)	this->shopSlots[i]->slotIndex = i;
}

void UShopSystem::AssignShopButtons(UButton * btnArmor, UButton * btnJewelry, UButton * btnPotions, UButton * btnShields, UButton * btnSpells,
	UButton * btnWeapons, UButton * btnNext, UButton * btnPrevious, UButton * btnCloseShop, UButton * btnCloseShopX)
{
	this->BTN_Armor = btnArmor;
	this->BTN_Jewelry = btnJewelry;
	this->BTN_Shields = btnShields;
	this->BTN_Weapons = btnWeapons;
	this->BTN_Potions = btnPotions;
	this->BTN_Spells = btnSpells;
	this->BTN_Next = btnNext;
	this->BTN_Previous = btnPrevious;
	this->BTN_CloseShop = btnCloseShop;
	this->BTN_CloseShopX = btnCloseShopX;


	this->BTN_Armor->WidgetStyle = this->BS_ButtonNotSelected;
	this->BTN_Jewelry->WidgetStyle = this->BS_ButtonNotSelected;
	this->BTN_Potions->WidgetStyle = this->BS_ButtonNotSelected;
	this->BTN_Shields->WidgetStyle = this->BS_ButtonNotSelected;
	this->BTN_Spells->WidgetStyle = this->BS_ButtonNotSelected;
	this->BTN_Weapons->WidgetStyle = this->BS_ButtonNotSelected;
}

void UShopSystem::AssignShopButtonsTextBlocks(UTextBlock * btnArmor, UTextBlock * btnJewelry, UTextBlock * btnPotions, UTextBlock * btnShields,
	UTextBlock * btnSpells, UTextBlock * btnWeapons)
{
	this->BTN_TXT_Armor = btnArmor;
	this->BTN_TXT_Jewelry = btnJewelry;
	this->BTN_TXT_Shields = btnShields;
	this->BTN_TXT_Weapons = btnWeapons;
	this->BTN_TXT_Potions = btnPotions;
	this->BTN_TXT_Spells = btnSpells;

	this->SFI_ButtonSelected = this->BTN_TXT_Armor->Font;
	this->SFI_ButtonSelected.Size = SELECTED_BUTTON_FONT_SIZE;
	this->SC_ButtonSelected = FSlateColor(FLinearColor(0.6f, 0.6f, 0.5f, 1.0f));

	this->SFI_ButtonNotSelected = this->BTN_TXT_Armor->Font;
	this->SFI_ButtonNotSelected.Size = NOT_SELECTED_BUTTON_FONT_SIZE;
	this->SC_ButtonNotSelected = FSlateColor(FLinearColor(0.619608, 0.4f, 0.015686, 1.0f));
}

/** On left mouse button clicked onto shop button, close shop */
void UShopSystem::OnClickedCloseShopButton()
{
	this->bIsOpened = false;
	this->UW_Shop->SetVisibility(ESlateVisibility::Hidden);
}

/** On left mouse button clicked on armor button, assign armor items into inventory widget */
void UShopSystem::OnClickedArmorsButton()
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("OPENING ARMORS"));
	if (!bIsOpenedArmorsSlot)
	{
		this->BTN_TXT_Armor->SetFont(SFI_ButtonSelected);
		this->BTN_TXT_Armor->SetColorAndOpacity(SC_ButtonSelected);

		UpdateStyleOfLastSelectedButton();
		ApplyArmorThread();
	}
}

void UShopSystem::OnClickedJewelryButton()
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("OPENING JEWELRY"));
	if (!bIsOpenedJewelrySlot)
	{
		this->BTN_TXT_Jewelry->SetFont(SFI_ButtonSelected);
		this->BTN_TXT_Jewelry->SetColorAndOpacity(SC_ButtonSelected);

		UpdateStyleOfLastSelectedButton();
		ApplyJewelryThread();
	}
}

/** On left mouse button clicked on shields button, assign shields items into inventory widget */
void UShopSystem::OnClickedShieldsButton()
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("OPENING SHIELDS"));
	if (!bIsOpenedShieldsSlot)
	{
		this->BTN_TXT_Shields->SetFont(SFI_ButtonSelected);
		this->BTN_TXT_Shields->SetColorAndOpacity(SC_ButtonSelected);

		UpdateStyleOfLastSelectedButton();
		ApplyShieldsThread();
	}
}

/** On left mouse button clicked on weapons button, assign weapons items into inventory widget */
void UShopSystem::OnClickedWeaponsButton()
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("OPENING WEAPONS"));
	if (!bIsOpenedWeaponsSlot)
	{
		this->BTN_TXT_Weapons->SetFont(SFI_ButtonSelected);
		this->BTN_TXT_Weapons->SetColorAndOpacity(SC_ButtonSelected);

		UpdateStyleOfLastSelectedButton();
		ApplyWeaponsThread();
	}
}

/** On left mouse button clicked on potions button, assign potions items into inventory widget */
void UShopSystem::OnClickedPotionsButton()
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("OPENING POTIONS"));
	if (!bIsOpenedPotionsSlot)
	{
		this->BTN_TXT_Potions->SetFont(SFI_ButtonSelected);
		this->BTN_TXT_Potions->SetColorAndOpacity(SC_ButtonSelected);

		UpdateStyleOfLastSelectedButton();
		ApplyPotionsThread();
	}
}

/** On left mouse button clicked on spells button, assign spells items into inventory widget */
void UShopSystem::OnClickedSpellsButton()
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("OPENING SPELLS"));
	if (!bIsOpenedSpellsSlot)
	{
		this->BTN_TXT_Spells->SetFont(SFI_ButtonSelected);
		this->BTN_TXT_Spells->SetColorAndOpacity(SC_ButtonSelected);

		UpdateStyleOfLastSelectedButton();
		ApplySpellsThread();
	}

	//this->VB_ShopContent->MyVerticalBox->ClearChildren();
	//this->VB_ShopContent->AddChildToVerticalBox(this->IMAGE_NPC);
	//this->VB_ShopContent->AddChildToVerticalBox(this->BTN_Armor);
}

/** On left mouse button clicked on previous, if there are more pages with items, go to the previous page */
void UShopSystem::OnClickedPreviousButton()
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("OPENING PREVIOUS PAGE"));
}

/** On left mouse button clicked on next, if there are more pages with items, go to the next page */
void UShopSystem::OnClickedNextButton()
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("OPENING NEXT PAGE"));
}



/** Set shop widget visibility to hidden */
void UShopSystem::CloseShop()
{
	this->UW_Shop->SetVisibility(ESlateVisibility::Hidden);
	this->bIsOpened = false;
}

/** Set shop widget visibility to visible */
void UShopSystem::OpenShop()
{
	this->UW_Shop->SetVisibility(ESlateVisibility::Visible);
	this->bIsOpened = true;
}

void UShopSystem::ApplyTextureFromNPC(UTexture2D * newShopTextureNPC)
{
	this->IMAGE_NPC->SetBrushFromTexture(newShopTextureNPC);
}

void UShopSystem::UpdateStyleOfLastSelectedButton()
{
	switch (lastSelectedButton)
	{
	case EShopLastSelectedButton::ARMORS:			
		BTN_Armor->WidgetStyle = BS_ButtonNotSelected; 
		BTN_TXT_Armor->SetFont(SFI_ButtonNotSelected);
		BTN_TXT_Armor->SetColorAndOpacity(SC_ButtonNotSelected);
		break;
	case EShopLastSelectedButton::JEWELRY:			
		BTN_Jewelry->WidgetStyle = BS_ButtonNotSelected;
		BTN_TXT_Jewelry->SetFont(SFI_ButtonNotSelected);
		BTN_TXT_Jewelry->SetColorAndOpacity(SC_ButtonNotSelected);
		break;
	case EShopLastSelectedButton::POTIONS:			
		BTN_Potions->WidgetStyle = BS_ButtonNotSelected;	
		BTN_TXT_Potions->SetFont(SFI_ButtonNotSelected);
		BTN_TXT_Potions->SetColorAndOpacity(SC_ButtonNotSelected);
		break;
	case EShopLastSelectedButton::SHIELDS:			
		BTN_Shields->WidgetStyle = BS_ButtonNotSelected;
		BTN_TXT_Shields->SetFont(SFI_ButtonNotSelected);
		BTN_TXT_Shields->SetColorAndOpacity(SC_ButtonNotSelected);
		break;
	case EShopLastSelectedButton::SPELLS:		
		BTN_Spells->WidgetStyle = BS_ButtonNotSelected;
		BTN_TXT_Spells->SetFont(SFI_ButtonNotSelected);
		BTN_TXT_Spells->SetColorAndOpacity(SC_ButtonNotSelected);
		break;
	case EShopLastSelectedButton::WEAPONS:		
		BTN_Weapons->WidgetStyle = BS_ButtonNotSelected;
		BTN_TXT_Weapons->SetFont(SFI_ButtonNotSelected);
		BTN_TXT_Weapons->SetColorAndOpacity(SC_ButtonNotSelected);
		break;
	}
}
