// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../LoDEnumTypes.h"
#include "Item.generated.h"


UCLASS()
class LOD_API AItem : public AActor
{
	GENERATED_BODY()

private:

	/***/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemDetails", meta = (AllowPrivateAccess = "true"))
	EItemPropertyType itemPropertyType;

	/***/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemDetails", meta = (AllowPrivateAccess = "true"))
	EItemType itemType;

	/***/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemDetails", meta = (AllowPrivateAccess = "true"))
	uint8 requiredLevel;

	/***/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemDetails", meta = (AllowPrivateAccess = "true"))
	uint8 strenght;

	/***/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemDetails", meta = (AllowPrivateAccess = "true"))
	uint8 agility;

	/***/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemDetails", meta = (AllowPrivateAccess = "true"))
	uint8 vitality;

	/***/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemDetails", meta = (AllowPrivateAccess = "true"))
	uint8 intelligence;

	/***/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemDetails", meta = (AllowPrivateAccess = "true"))
	uint8 spirit;

	/***/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemDetails", meta = (AllowPrivateAccess = "true"))
	uint8 armor;

	/***/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemDetails", meta = (AllowPrivateAccess = "true"))
	uint8 magicResistance;

	/***/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemDetails", meta = (AllowPrivateAccess = "true"))
	uint8 minimalDamage;

	/***/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemDetails", meta = (AllowPrivateAccess = "true"))
	uint8 maximalDamage;

	/***/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemDetails", meta = (AllowPrivateAccess = "true"))
	uint8 valueGolds;
	
	/***/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemDetails", meta = (AllowPrivateAccess = "true"))
	uint8 valueSilvers;

	/***/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemDetails", meta = (AllowPrivateAccess = "true"))
	uint8 valueBronzes;

	/***/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemDetails", meta = (AllowPrivateAccess = "true"))
	uint32 bIsEquipable : 1;

	/** Store item name*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemDetails", meta = (AllowPrivateAccess = "true"))
	FString name;

public: /** Variables */

	/**		*/
	UPROPERTY()
	USceneComponent * Root;

	/**		*/
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent * Mesh;

	/** Used for boots, and gloves, as they need to be separeted object */
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent * MeshAdditional;

	/**		*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDetails")
	UTexture2D * itemTexture;

	/**		*/
	UPROPERTY()	class UWidgetComponent * itemNameWidget;

	/** Pick up sound effect */
	UPROPERTY(EditDefaultsOnly, Category = "ItemDetails")
	USoundBase * AC_PickUp;

protected:

	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;


public:

	/** Outline  the object */
	UFUNCTION()	void OutlineObject(UPrimitiveComponent * TouchedComponent);

	/** Do not outline the object */
	UFUNCTION()	void CancelOutlineObject(UPrimitiveComponent * TouchedComponent);

	/** Check the pick up */
	UFUNCTION()	void SetItemToPickUp(AActor * ClickedComp, FKey ButtonPressed);

	/** Pick up the item*/
	UFUNCTION()	void PickUpItem();

public: /** FORCEINLINE Functions */

	/** Getter Functions */
	FORCEINLINE const EItemPropertyType GetItemPropertyType()		{ return this->itemPropertyType;  }
	FORCEINLINE	const EItemType GetItemType()						{ return this->itemType;  }
	FORCEINLINE	const uint8 GetRequiredLevel()						{ return this->requiredLevel; }
	FORCEINLINE const uint8 GetGoldsValue()							{ return this->valueGolds; }
	FORCEINLINE	const uint8 GetSilversValue()						{ return this->valueSilvers; }
	FORCEINLINE	const uint8 GetStrenght()							{ return this->strenght; }
	FORCEINLINE	const uint8 GetAgility()							{ return this->agility; }
	FORCEINLINE	const uint8 GetVitality()							{ return this->vitality; }
	FORCEINLINE	const uint8 GetIntelligence()						{ return this->intelligence; }
	FORCEINLINE	const uint8 GetSpirit()								{ return this->spirit; }
	FORCEINLINE	const uint8 GetArmor()								{ return this->armor; }
	FORCEINLINE	const uint8 GetMagicResistance()					{ return this->magicResistance; }
	FORCEINLINE	const uint8 GetMinimalDamge()						{ return this->minimalDamage; }
	FORCEINLINE	const uint8 GetMaximalDamage()						{ return this->maximalDamage; }
	FORCEINLINE	const bool IsEquipable()							{ return this->bIsEquipable; }

public:	/** Constructor */

	/** Default constructor */
	AItem();
};


