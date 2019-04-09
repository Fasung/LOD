// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/NPC.h"
#include "NPC_Shop.generated.h"


/**
 * 
 */
UCLASS()
class LOD_API ANPC_Shop : public ANPC
{
	GENERATED_BODY()


private:
	/** Store the shop type, could be alchemist, blacksmith etc.. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ShopDetails", meta = (AllowPrivateAccess = "true"))
	EShopType shopType;

	/**  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ShopDetails", meta = (AllowPrivateAccess = "true"))
	uint8 itemsStrongness;

	/** Determine whhat type the NPC is, could be enemy, friendly or neutral */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShopDetails", meta = (AllowPrivateAccess = "true"))
	class UShopItems * shopItems;
	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public: /** FORCEINLINE Functions */

	/** Return address of shopItems structure */
	FORCEINLINE class UShopItems * GetShopItems()	const			{ return this->shopItems; }

public:	/** Constructors */
		/** Sets default values for this pawn's properties */
	ANPC_Shop();
};
