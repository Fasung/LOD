// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../LoDEnumTypes.h"
#include "Blueprint/UserWidget.h"
#include "NPC_Name.generated.h"

/**
 * 
 */
UCLASS()
class LOD_API UNPC_Name : public UUserWidget
{
	GENERATED_BODY()
	

private: /** Properties */

	 /** Store item name, use to set text in blueprint widget inherited from this class */
	UPROPERTY(BlueprintReadOnly, Category = "NPC_Widget", meta = (AllowPrivateAccess = "true"))
	FString npcName;

	/** Item type could be common, uncommon, rare or epic, it determine what colour will be text drew */
	UPROPERTY(BlueprintReadOnly, Category = "NPC_Widget", meta = (AllowPrivateAccess = "true"))
	ENPC_Type npcType;

	/** Top down camera */
	UPROPERTY(BlueprintReadOnly, Category = "NPC_Widget", meta = (AllowPrivateAccess = "true"))
	class UAttributeSystem * attributeSystem;

public: /** Functions */

	FORCEINLINE void SetUpWidget(FString newName, ENPC_Type newNPCType, class UAttributeSystem * newAttributeSystem)
	{
		this->npcName = newName;
		this->npcType = newNPCType;
		this->attributeSystem = newAttributeSystem;
	}
};
