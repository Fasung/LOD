// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipmentSystem.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LOD_API UEquipmentSystem : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY()
	class UInventorySlot * inventorySlotWhichContaintPotion;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EquipmentSlots", meta = (AllowPrivateAccess = "true"))
	class UInventorySlot * helmetSlot;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EquipmentSlots", meta = (AllowPrivateAccess = "true"))
	class UInventorySlot * chestSlot;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EquipmentSlots", meta = (AllowPrivateAccess = "true"))
	class UInventorySlot * glovesSlot;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EquipmentSlots", meta = (AllowPrivateAccess = "true"))
	class UInventorySlot * pantsSlot;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EquipmentSlots", meta = (AllowPrivateAccess = "true"))
	class UInventorySlot * bootsSlot;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EquipmentSlots", meta = (AllowPrivateAccess = "true"))
	class UInventorySlot * rightHandSlot;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EquipmentSlots", meta = (AllowPrivateAccess = "true"))
	class UInventorySlot * leftHandSlot;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EquipmentSlots", meta = (AllowPrivateAccess = "true"))
	class UInventorySlot * ringOneSlot;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EquipmentSlots", meta = (AllowPrivateAccess = "true"))
	class UInventorySlot * ringTwoSlot;

	UPROPERTY()	class USkeletalMeshComponent * characterMesh;

	UPROPERTY()	class UAttributeSystem * attributeSystem;

private: /** Functions */

	UFUNCTION() void IncreaseAttributes(class AItem * item);

	UFUNCTION() void DecreaseAttributes(class AItem * item);

public: /** Functions */

	UFUNCTION()	void EquipItem(class UInventorySlot * newEquipment);

	UFUNCTION()	void SwapItemWithInventoryItem(class UInventorySlot * newEquipment);

	UFUNCTION() void UnEquipItem(class UInventorySlot * oldEquipment);

	UFUNCTION() const bool HasEquipedItemInSlot(class AItem * newItem);


public: /** FORCEINLINE Functions */

	FORCEINLINE	void AttachSkeletalMesh(class USkeletalMeshComponent * mesh)				{ this->characterMesh = mesh; }
	
	FORCEINLINE	void AttachAttributeSystem(class UAttributeSystem * newAttributeSystem)		{ this->attributeSystem = newAttributeSystem; }

	FORCEINLINE	void AttachPotionToDrink(class AItem * potion);

	FORCEINLINE	void StartDrinkPotion(class UInventorySlot * slotWithPotion);

	FORCEINLINE	void StopDrinkingPotion();


public: /** Constructor */
	/** Sets default values for this component's properties */
	UEquipmentSystem();
};
