// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LoDEnumTypes.generated.h"

/**
 * 
 */



UENUM(BlueprintType)		
enum class EShopType : uint8
{
	BLACKSMITH 	UMETA(DisplayName = "Blacksmith"),
	ALCHEMIST 	UMETA(DisplayName = "Alchemist")
};

UENUM(BlueprintType)
enum class ECurrentLocation : uint8
{
	LVL2 		UMETA(DisplayName = "LVL2"),
	LVL3_Part1 	UMETA(DisplayName = "LVL3_Part1"),
	LVL3_Part2 	UMETA(DisplayName = "LVL3_Part2"),
	LVL4_Part1 	UMETA(DisplayName = "LVL4_Part1"),
	LVL5 		UMETA(DisplayName = "LVL5")
};

UENUM(BlueprintType)
enum class EAIBehaviourState : uint8
{
	WANDERING 	UMETA(DisplayName = "Wandering"),
	IDLE 		UMETA(DisplayName = "Idle"),
	CHASING		UMETA(DisplayName = "Chasing"),
	ATTACKING	UMETA(DisplayName = "Attacking"),
	DEAD		UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum class ENPC_Type : uint8
{
	ENEMY		UMETA(DisplayName = "Enemy"),
	FRIENDLY	UMETA(DisplayName = "Friendly"),
	SHOP		UMETA(DisplayName = "Shop")
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	COMMON		UMETA(DisplayName = "Common"),
	UNCOMMON	UMETA(DisplayName = "Uncommon"),
	RARE		UMETA(DisplayName = "Rare"),
	EPIC		UMETA(DisplayName = "Epic"),
	ANCIENT		UMETA(DisplayName = "Ancient")
};

UENUM(BlueprintType)
enum class EFloatingTextType : uint8
{
	DAMAGE_DONE			UMETA(DisplayName = "DamageDone"),
	EXPERIENCE_GAINED	UMETA(DisplayName = "ExperienceGained"),
	HEALTH_GAINED		UMETA(DisplayName = "HealthGained"),
	MANA_GAINED			UMETA(DisplayName = "ManaGained"),
	DAMAGE_DONE_ENEMY	UMETA(DisplayName = "DamageDoneEnemy")
};

UENUM(BlueprintType)
enum class EItemPropertyType : uint8
{
	HELMET					UMETA(DisplayName = "Helmet"),
	CHEST					UMETA(DisplayName = "Chest"),
	GLOVES					UMETA(DisplayName = "Gloves"),
	PANTS					UMETA(DisplayName = "Pants"),
	BOOTS					UMETA(DisplayName = "Boots"),
	ONE_HAND_WEAPON			UMETA(DisplayName = "OneHandWeapon"),
	TWO_HAND_WEAPON			UMETA(DisplayName = "TwoHandWeapon"),
	SHIELD					UMETA(DisplayName = "Shield"),
	RING					UMETA(DisplayName = "Ring"),
	NECKLACE				UMETA(DisplayName = "Necklace"),
	MANA_POTION				UMETA(DisplayName = "ManaPotion"),
	HEALTH_POTION			UMETA(DisplayName = "HealthPotion"),
	MONEY_GOLD				UMETA(DisplayName = "Golds"),
	MONEY_SILVER			UMETA(DisplayName = "Silvers"),
	MONEY_BRONZE			UMETA(DisplayName = "Bronzes")
};

UENUM()
enum class EDragDropSlotType : uint8
{
	HEAD_SLOT = 0,
	CHEST_SLOT = 1,
	GLOVES_SLOT = 2,
	PANTS_SLOT = 3,
	BOOTS_SLOT = 4,
	LEFT_HAND_SLOT = 7,
	RIGHT_HAND_SLOT = 5,
	RING_SLOT = 8,
	INVENTORY_SLOT = 20,
	SHOP_SLOT = 21
};

UENUM()
enum class ETargetType : uint8
{
	NO_TARGET,
	TREASURE_CHEST,
	TREASURE_DESTRUCTIBLE,
	ITEM,
	ELEVATOR,
	NPC
};

UENUM()
enum class EShopLastSelectedButton : uint8
{
	ARMORS,
	JEWELRY,
	POTIONS,
	SHIELDS,
	SPELLS,
	WEAPONS,
	NONE
};

