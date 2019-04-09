// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AttributesCalculator.generated.h"

UCLASS()
class LOD_API AAttributesCalculator : public AActor
{
	GENERATED_BODY()

public:
	/** Used to calculate player health, player vitality is multiplied by VITALITY_MULTIPIER and result is final player's maximal health*/
	static const uint8 VITALITY_MULTIPIER = 35;
	/** Used to calculate player mana, player intelligence is multiplied by INTELLIGENCE_MULTIPIER and result is final player's maximal mana*/
	static const uint8 INTELLIGENCE_MULTIPIER = 28;

	/** Used to calculate player critical chance, contain 11 numbers, for 10 levels
	* How to calculate critial chance: current agility / AGILITY[playerLevel] will give you percentage from 0% to 100% */
	static const int TABLE_AGILITY_TO_CRITICAL_CHANCE[11];// = { 0, 107, 113, 121, 134, 142, 152, 160, 170, 180, 190 };
	// Maximum level is 10, if the value is higher than 10, it will return error
	static const int TABLE_EXPERIENCE_TO_NEXT_LEVEL[11];// = { 0, 400, 540, 720, 920, 1240, 1680, 2020, 2760, 3460, 4800 };

private:
	// Sets default values for this actor's properties
	AAttributesCalculator();	
};
