// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Public/Item.h"
#include "ItemDragDropOperation.h"
#include "Components/Image.h"
#include "../LoDEnumTypes.h"
#include "InventorySlot.generated.h"

/**
 * 
 */
UCLASS()
class LOD_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/** ADD COMENT *//** MAKE IT UINT8 */
	/** ADD COMENT */	/** MAKE IT UINT8 */
	/** MAKE IT UINT8 *//** MAKE IT UINT8 */
	/** MAKE IT UINT8 *//** MAKE IT UINT8 */
	/** MAKE IT UINT8 *//** MAKE IT UINT8 */
	/** MAKE IT UINT8 *//** MAKE IT UINT8 */
	/** MAKE IT UINT8 *//** MAKE IT UINT8 */
	/** MAKE IT UINT8 *//** MAKE IT UINT8 */
	/** ADD COMENT *//** MAKE IT UINT8 */
	/** ADD COMENT *//** MAKE IT UINT8 */
	UPROPERTY(BlueprintReadWrite, Category = "SlotProperties", meta = (AllowPrivateAccess = "true"))
	float slotIndex;

private:
	/** ADD COMENT */
	/** ADD COMENT */
	/** ADD COMENT */
	UPROPERTY(EditAnywhere, Category = "SlotProperties", meta = (AllowPrivateAccess = "true"))
	EDragDropSlotType slotType;
	/** ADD COMENT */
	/** ADD COMENT */
	/** ADD COMENT */
	UPROPERTY(BlueprintReadOnly, Category = "SlotProperties", meta = (AllowPrivateAccess = "true"))
	AItem * item;

	UPROPERTY() UImage * itemImage;

private:
	UItemDragDropOperation * CreateItemDragDrop() const;
	
public:
	/** Return item in inventory slot */
	FORCEINLINE AItem * GetItem()							{ return this->item; }

	/** Return slot type */
	const FORCEINLINE EDragDropSlotType GetSlotType()		{ return this->slotType; }


	FORCEINLINE void SetItem(AItem * newItem)				{ this->item = newItem; }
	FORCEINLINE void SetItemToNull()						{ this->item = NULL; }
	FORCEINLINE void ResetRenderScale()						{ this->itemImage->SetRenderScale(FVector2D(1.0f, 1.0f)); }
	FORCEINLINE void SetRenderScale(float x, float y)		{ this->itemImage->SetRenderScale(FVector2D(x, y)); }
	FORCEINLINE void SetBrushFromItemTexture()				{ this->itemImage->SetBrushFromTexture(this->item->itemTexture, true); }


	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;



	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;



	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;




	//virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	//virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	/** Assign close inventory button into C++ button pointer */
	/** Assign close inventory button into C++ button pointer */
	/** Assign close inventory button into C++ button pointer */
	/** Assign close inventory button into C++ button pointer */
	/** Assign close inventory button into C++ button pointer */

	UFUNCTION(BlueprintCallable, Category = "InventoryUtilityFunctions")
	void AssignImage(UImage * newImage);
};

