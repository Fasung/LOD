// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../LoDEnumTypes.h"
#include "ItemName.generated.h"

/**
 * 
 */
UCLASS()
class LOD_API UItemName : public UUserWidget
{
	GENERATED_BODY()

private: /** Properties */

	/** Store item name, use to set text in blueprint widget inherited from this class */
	UPROPERTY(BlueprintReadOnly, Category = "ItenNameWidget", meta = (AllowPrivateAccess = "true"))
	FString itemName;

	/** Item type could be common, uncommon, rare or epic, it determine what colour will be text drew */
	UPROPERTY(BlueprintReadOnly, Category = "ItenNameWidget", meta = (AllowPrivateAccess = "true"))
	EItemType itemType;

public: /** Functions */

	FORCEINLINE void SetUpWidget(FString newName, EItemType newItemType)
	{ 
		this->itemName = newName; 
		this->itemType = newItemType;
	}
};
