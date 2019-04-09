// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventorySystem.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LOD_API UInventorySystem : public UActorComponent
{
	GENERATED_BODY()

private: /** Variables */

	/** Store the inventory slots count*/
	uint8 totalSlotsCount;
	
	/** Store the value of avalaible slots inside the inventory*/
	uint8 emptySlots;

	/** */
	uint8 countOfHealthPotions;

	/** */
	uint8 countOfManaPotions;

	/** Store pointer to inventory interface */
	UPROPERTY() class UUserWidget * UW_Inventory;

	/** Store pointer to button close inventory  */
	UPROPERTY() class UButton * BTN_CloseInventory;

	/** Store the golds value*/
	UPROPERTY(BlueprintReadOnly, Category = "Money", meta = (AllowPrivateAccess = "true"))
	int golds;

	/** Store the golds value*/
	UPROPERTY(BlueprintReadOnly, Category = "Money", meta = (AllowPrivateAccess = "true"))
	int silvers;
	
	/** Store inventory status */
	UPROPERTY(BlueprintReadOnly, Category = "InventoryStatus", meta = (AllowPrivateAccess = "true"))
	uint32 bIsOpened : 1;

	/** Store array of inventory slots, each slot containt item pointer, item image */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "IsInventorySlotEmpty", meta = (AllowPrivateAccess = "true"))
	TArray<class UInventorySlot*> inventorySlots;

public: /** C++ Functions */

	/** Pick up item and put him inside the item array */
	UFUNCTION()	bool PickUpItem(class AItem * newItem);

	/** Remove item from inventory array, item has been equiped */
	UFUNCTION()	void RemoveItemFromSlot(class UInventorySlot * itemSlot);

	/** Destroy item from inventory array, it has been used, drinked */
	UFUNCTION()	void DestroyItemFromSlot(class UInventorySlot * itemSlot);

	/** Throw away item and remove him from inventory array */
	UFUNCTION()	void ThrowItem(class AItem * newItem);

	/** Sell item and remove him from inventory array, addjust money from item price */
	UFUNCTION()	void SellItem(class UInventorySlot * itemSlot);

	/** If player has enough money and space, item will be added into inventory system,
	and money will be decreased, if not or has not enough space, play audio */
	UFUNCTION()	bool CanBuyItem(class UInventorySlot * itemSlot);

public:	/** BlueprintCallable Functions */

	/** Assign inventory blueprint widget into C++ code */
	UFUNCTION(BlueprintCallable, Category = "InventoryUtilityFunctions")
	void AssignInventoryWidget(class UUserWidget * newInventoryWidget);

	/** Assign inventory slots blueprint widgets into C++ array */
	UFUNCTION(BlueprintCallable, Category = "InventoryUtilityFunctions")
	void AssignInventorySlots(TArray<class UInventorySlot*> newInventorySlots);

	/** Assign close inventory button into C++ button pointer */
	UFUNCTION(BlueprintCallable, Category = "InventoryUtilityFunctions")
	void AssignCloseInventoryButton(class UButton * newButton);

public: /** FORCEINLINE Functions */


	/** Called from drag and drop operation, item from equipment system was moved to inventory */
	FORCEINLINE void DecreaseCountOfEmptySlotsBy(uint8 amount) { this->emptySlots -= amount; }

	/** Return inventory status, return bIsOpened variable */
	FORCEINLINE	const uint32 IsOpened() { return this->bIsOpened; }

	/** Open inventory, set bIsOpened variable to true and inventory widget visibility to visible */
	FORCEINLINE void OpenInventory();

	/** Close inventory, set bIsOpened variable to false and inventory widget visibility to hidden */
	FORCEINLINE void CloseInventory();

	/** */
	FORCEINLINE bool HasHealthPotion()				{ if (this->countOfHealthPotions > 0) return true; else return false; }

	/** */
	FORCEINLINE bool HasManaPotion()				{ if (this->countOfManaPotions > 0) return true; else return false; }

	class UInventorySlot * GetHealthPotion() const;

	class UInventorySlot * GetManaPotion() const ;

	FORCEINLINE void DecreaseHealthPotionsCount()	{ countOfHealthPotions--; }

	FORCEINLINE void DecreaseManaPotionsCount()		{ countOfManaPotions--; }


public: /** Dynamics Functions*/

	/** On left mouse button clicked onto inventory button, close inventory */
	UFUNCTION()	void OnClickedCloseInventoryButton();

protected:
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

public: /** Constructors */

	/** Sets default values for this component's properties */
	UInventorySystem();
};
