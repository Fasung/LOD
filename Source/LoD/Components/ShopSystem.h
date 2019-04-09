// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Public/Styling/SlateTypes.h"
#include "../LoDEnumTypes.h"
#include "ShopSystem.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LOD_API UShopItems : public UActorComponent
{
	GENERATED_BODY()

public:
	/** */
	UPROPERTY()	TArray<class AItem * > itemsArmors;

	/** */
	UPROPERTY()	TArray<class AItem * > itemsJewelry;

	/** */
	UPROPERTY()	TArray<class AItem * > itemsPotions;

	/** */
	UPROPERTY()	TArray<class AItem * > itemsShields;

	/** */
	UPROPERTY()	TArray<class AItem * > itemsSpells;

	/** */
	UPROPERTY()	TArray<class AItem * > itemsWeapons;



	/**  */
	UPROPERTY()	uint32 bHasArmors : 1;

	/**  */
	UPROPERTY()	uint32 bHasJewelry : 1;
	 
	/**  */
	UPROPERTY()	uint32 bHasPotions : 1;

	/**  */
	UPROPERTY()	uint32 bHasShields : 1;

	/**  */
	UPROPERTY()	uint32 bHasSpells : 1;	

	/**  */
	UPROPERTY()	uint32 bHasWeapons : 1;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LOD_API UShopSystem : public UActorComponent
{
	GENERATED_BODY()

private: /** Variables */

	/** Store pointer to shop interface */
	UPROPERTY()	class UUserWidget * UW_Shop;

	/** Store pointer to vertical box, which store: 
	* 1. NPC IMAGE, image is static or live record of NPC
	* 2. Buttons, this is variable, it could be just one button or even 5, depends on how many kinds
	*	 of items shop contain. The max possible number is up to number of shop threads (arrays of items)
	* 3. BUTTON_CloseShop	*/
	UPROPERTY() class UVerticalBox * VB_ShopContent;

	/** Store pointer to shop interface */
	UPROPERTY()	class UImage * IMAGE_NPC;

	/** Store pointer to button armor  */
	UPROPERTY() class UButton * BTN_Armor;
	UPROPERTY() class UTextBlock * BTN_TXT_Armor;

	/** Store pointer to button weapons  */
	UPROPERTY() class UButton * BTN_Jewelry;
	UPROPERTY() class UTextBlock * BTN_TXT_Jewelry;

	/** Store pointer to button weapons  */
	UPROPERTY() class UButton * BTN_Potions;
	UPROPERTY() class UTextBlock * BTN_TXT_Potions;

	/** Store pointer to button shields  */
	UPROPERTY() class UButton * BTN_Shields;
	UPROPERTY() class UTextBlock * BTN_TXT_Shields;

	/** Store pointer to button weapons  */
	UPROPERTY() class UButton * BTN_Spells;
	UPROPERTY() class UTextBlock * BTN_TXT_Spells;

	/** Store pointer to button weapons  */
	UPROPERTY() class UButton * BTN_Weapons;
	UPROPERTY() class UTextBlock * BTN_TXT_Weapons;

	/** Store pointer to button next  */
	UPROPERTY() class UButton * BTN_Next;

	/** Store pointer to button previous  */
	UPROPERTY() class UButton * BTN_Previous;

	/** Store pointer to button close shope  */
	UPROPERTY() class UButton * BTN_CloseShop;

	/** Store pointer to button close shope  */
	UPROPERTY() class UButton * BTN_CloseShopX;

	/** Structures with visualization data of selected shop button */
	UPROPERTY() FButtonStyle BS_ButtonSelected;
	UPROPERTY() FSlateFontInfo SFI_ButtonSelected;
	UPROPERTY() FSlateColor SC_ButtonSelected;
	
	/** Structures with visualization data of not selected shop button */
	UPROPERTY() FButtonStyle BS_ButtonNotSelected;
	UPROPERTY() FSlateFontInfo SFI_ButtonNotSelected;
	UPROPERTY() FSlateColor SC_ButtonNotSelected;

	/** Boolean variable, prevent from loading items from array if the current slot is already opened */
	UPROPERTY()	uint32 bIsOpenedArmorsSlot : 1;

	/** Boolean variable, prevent from loading items from array if the current slot is already opened */
	UPROPERTY()	uint32 bIsOpenedJewelrySlot : 1;

	/** Boolean variable, prevent from loading items from array if the current slot is already opened */
	UPROPERTY()	uint32 bIsOpenedShieldsSlot : 1;

	/** Boolean variable, prevent from loading items from array if the current slot is already opened */
	UPROPERTY()	uint32 bIsOpenedWeaponsSlot : 1;

	/** Boolean variable, prevent from loading items from array if the current slot is already opened */
	UPROPERTY()	uint32 bIsOpenedPotionsSlot : 1;

	/** Boolean variable, prevent from loading items from array if the current slot is already opened */
	UPROPERTY()	uint32 bIsOpenedSpellsSlot : 1;

	/** Store shop status */
	UPROPERTY()	uint32 bIsOpened : 1;

	/** Store the data about last selected button*/
	EShopLastSelectedButton lastSelectedButton;

public: /** Variables */

	/**	Store pointer to the last selected shop NPC's items */
	UPROPERTY()	class UShopItems * shopItems;

	/** Store array of shop slots, each slot containt item pointer, item image */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ShopUtilityFunctions", meta = (AllowPrivateAccess = "true"))
	TArray<class UInventorySlot*> shopSlots;

	/** Used to calculate automatic closing shop, depends on distance from player */
	FVector shopPosition;

	/** Used to calculate distance from player, coordinate X,Y keep target horizontal displacement, coordinate Y store not squared distance */
	FVector shopDistanceToPlayer;

private: /** C++ Functions */

	/** Assign items from armor array to inventory widget */
	UFUNCTION() void ApplyArmorThread();

	/** Assign items from shields array to inventory widget */
	UFUNCTION() void ApplyJewelryThread();

	/** Assign items from shields array to inventory widget */
	UFUNCTION() void ApplyShieldsThread();

	/** Assign items from weapons array to inventory widget */
	UFUNCTION() void ApplyWeaponsThread();

	/** Assign items from potions array to inventory widget */
	UFUNCTION() void ApplyPotionsThread();

	/** Assign items from spells array to inventory widget */
	UFUNCTION() void ApplySpellsThread();

	/*
	
	DO COMMMEENTSSSS
	DO COMMMEENTSSSS
	DO COMMMEENTSSSS
	DO COMMMEENTSSSS
	DO COMMMEENTSSSS
	DO COMMMEENTSSSS
	DO COMMMEENTSSSS

	*/
	FORCEINLINE void UpdateStyleOfLastSelectedButton();

protected: /** C++ Virtual Functions */

	/** Called when the game starts */
	virtual void BeginPlay() override;

public: /** C++ Functions */

	/** Apply first available shop thread is called from targeting system, it check what kind of items shop containts
	* and apply first one as a default, the threads are checked in following order:	Armors, Shields, Weapons, Potions, Spells */
	UFUNCTION() void ApplyFirstAvailableShopThread();

	/** Remove item from the shop. Passed item slot parameter contain the item position in item array, it is used to remove the item from the array */
	UFUNCTION() void RemoveItemFromShop(class UInventorySlot * shopSlot);

	/** Item from the action slot will be added into shop system */
	UFUNCTION() void AddItemToShop(class UInventorySlot * sellingItemSlot);

public:	/** BlueprintCallable Functions */

	/** Assign shop blueprint widget into C++ code */
	UFUNCTION(BlueprintCallable, Category = "ShopUtilityFunctions")
	void AssignShopWidget(class UUserWidget * newShopWidget, class UImage * imageNPC, class UVerticalBox * verticalBox);

	/** Assign shop slots blueprint widgets into C++ array */
	UFUNCTION(BlueprintCallable, Category = "ShopUtilityFunctions")
	void AssignShopSlots(TArray<class UInventorySlot*> newShopSlots);

	/** Assign shop buttons into shop system button pointers, in following order:
	* 1. BUTTON_Armor
	* 2. BUTTON_Jewelry
	* 3. BUTTON_Potions
	* 4. BUTTON_Shields
	* 5. BUTTON_Spells
	* 6. BUTTON_Weapons
	* 7. BUTTON_Next
	* 8. BUTTON_Previous
	* 9. BUTTON_CloseShop
	* 10. BUTTON_CloseShopX	*/
	UFUNCTION(BlueprintCallable, Category = "ShopUtilityFunctions")
	void AssignShopButtons(class UButton * btnArmor, class UButton * btnJewelry, class UButton * btnPotions, class UButton * btnShields, class UButton * btnSpells,
		class UButton * btnWeapons, class UButton * btnNext, class UButton * btnPrevious, class UButton * btnCloseShop, class UButton * btnCloseShopX);

	/** Assign shop buttons into shop system button pointers, in following order:
	* 1. BUTTON_Armor
	* 2. BUTTON_Jewelry
	* 3. BUTTON_Potions
	* 4. BUTTON_Shields
	* 5. BUTTON_Spells
	* 6. BUTTON_Weapons	*/
	UFUNCTION(BlueprintCallable, Category = "ShopUtilityFunctions")
	void AssignShopButtonsTextBlocks(class UTextBlock * btnArmor, class UTextBlock * btnJewelry, class UTextBlock * btnPotions, class UTextBlock * btnShields, 
		class UTextBlock * btnSpells, class UTextBlock * btnWeapons);

public: /** FORCEINLINE Functions */

	/** Create new brush for variable IMAGE_NPC from texture which is store in NPC, this function is called when player click on shop NPC */
	FORCEINLINE void ApplyTextureFromNPC(class UTexture2D * newShopTextureNPC);

	/** Return shop status, return bIsOpened variable */
	FORCEINLINE	const uint32 IsOpened() { return this->bIsOpened; }

	/** Open shop, set bIsOpened variable to false and shop widget visibility to hidden */
	FORCEINLINE void CloseShop();

	/** Open shop, set bIsOpened variable to true and shop widget visibility to visible */
	FORCEINLINE void OpenShop();

public: /** Dynamics Functions*/

	/** On left mouse button clicked on close shop button, close shop */
	UFUNCTION()	void OnClickedCloseShopButton();

	/** On left mouse button clicked on armor button, assign armor items into inventory widget */
	UFUNCTION()	void OnClickedArmorsButton();

	/** On left mouse button clicked on potions button, assign potions items into inventory widget */
	UFUNCTION()	void OnClickedJewelryButton();

	/** On left mouse button clicked on potions button, assign potions items into inventory widget */
	UFUNCTION()	void OnClickedPotionsButton();

	/** On left mouse button clicked on shields button, assign shields items into inventory widget */
	UFUNCTION()	void OnClickedShieldsButton();

	/** On left mouse button clicked on spells button, assign spells items into inventory widget */
	UFUNCTION()	void OnClickedSpellsButton();

	/** On left mouse button clicked on weapons button, assign weapons items into inventory widget */
	UFUNCTION()	void OnClickedWeaponsButton();

	/** On left mouse button clicked on previous, if there are more pages with items, go to the previous page */
	UFUNCTION()	void OnClickedPreviousButton();

	/** On left mouse button clicked on next, if there are more pages with items, go to the next page */
	UFUNCTION()	void OnClickedNextButton();

public: /** Constructor */

	/** Sets default values for this component's properties */
	UShopSystem();
};
