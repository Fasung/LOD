// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputCoreTypes.h"
#include "Components/ActorComponent.h"
#include "ControlsSettings.generated.h"

/**enum EKeyActions
{
	ABILITY_ONE,
	ABILITY_TWO,
	ABILITY_THREE,
	ABILITY_FOUR,
	ABILITY_FIVE,
	DRINK_HEALTH_POTION,
	DRINK_MANA_POTION,
	OPEN_INVENTORY,
	OPEN_PAUSE_MENU,
	OPEN_QUEST_LOG,
	OPEN_SPEEL_BOOK
};
*/

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LOD_API UControlsSettings : public UActorComponent
{
	GENERATED_BODY()
public:
	/*	FKey KeyActions[11] =	{	};	*/

	FKey AbilityOne = EKeys::One;
	FKey AbilityTwo = EKeys::Two;
	FKey AbilityThree = EKeys::Three;
	FKey AbilityFour = EKeys::Four;
	FKey AbilityFive = EKeys::Five;

	/** Other action keys */
	FKey DrinkHealthPotion = EKeys::H;
	FKey DrinkManaPotion = EKeys::M;

	/** User Interface keys */
	FKey OpenInventory = EKeys::I;
	FKey OpenPauseMenu = EKeys::Escape;
	FKey OpenQuestLog = EKeys::Q;
	FKey OpenSpellBook = EKeys::S;
	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
public:
	// Sets default values for this component's properties
	UControlsSettings();
};
