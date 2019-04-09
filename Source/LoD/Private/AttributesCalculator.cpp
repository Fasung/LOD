// Fill out your copyright notice in the Description page of Project Settings.

#include "AttributesCalculator.h"

const uint8 AAttributesCalculator::VITALITY_MULTIPIER;// = 35;
const uint8 AAttributesCalculator::INTELLIGENCE_MULTIPIER;// = 28;
const int AAttributesCalculator::TABLE_AGILITY_TO_CRITICAL_CHANCE[11] = { 0, 107, 113, 121, 134, 142, 152, 160, 170, 180, 190 };
const int AAttributesCalculator::TABLE_EXPERIENCE_TO_NEXT_LEVEL[11] = { 0, 400, 540, 720, 920, 1240, 1680, 2020, 2760, 3460, 4800 };

// Sets default values
AAttributesCalculator::AAttributesCalculator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}