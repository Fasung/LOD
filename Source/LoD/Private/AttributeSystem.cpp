// Fill out your copyright notice in the Description page of Project Settings.

#include "AttributeSystem.h"
#include "LoDCharacter.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"


// Sets default values for this component's properties
UAttributeSystem::UAttributeSystem()	:
	strength(12),
	agility(9),
	vitality(14),
	intelligence(8),
	spirit(7),
	armor(0),
	magicResistance(0),
	currentLevel(1),
	currentExperience(0),
	experienceToNextLevel(AAttributesCalculator::TABLE_EXPERIENCE_TO_NEXT_LEVEL[1])
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	maximumHealth = vitality * AAttributesCalculator::VITALITY_MULTIPIER;
	currentHealth = 10;// m_maximumHealth;
	maximumMana = intelligence * AAttributesCalculator::INTELLIGENCE_MULTIPIER;
	currentMana = maximumMana / 2.0f; // m_maximumMana;
}

void UAttributeSystem::BeginPlay()
{
	Super::BeginPlay();

	this->currentHealth = this->maximumHealth;
}

void UAttributeSystem::IncreaseExperience(int addjustExperience)
{
	this->currentExperience += addjustExperience;

	if (this->currentExperience > this->experienceToNextLevel - 1)
	{
		this->currentExperience -= this->experienceToNextLevel;
		this->experienceToNextLevel = AAttributesCalculator::TABLE_EXPERIENCE_TO_NEXT_LEVEL[this->currentLevel];
		this->currentLevel++;

		Cast<ALoDCharacter>(GetOwner())->PlaySoundEffectLevelUP();
		if (PS_levelUpEffect) PS_levelUpEffect->ActivateSystem();
	}
}

// Called every frame
void UAttributeSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	// TODO: DELETE !!! UPDATE ONLY WHEN PLAYER REACHED NEW LEVEL OR IS WEARING NEW CLOTHS
	// TODO: DELETE !!! UPDATE ONLY WHEN PLAYER REACHED NEW LEVEL OR IS WEARING NEW CLOTHS
	// TODO: DELETE !!! UPDATE ONLY WHEN PLAYER REACHED NEW LEVEL OR IS WEARING NEW CLOTHS
	// TODO: DELETE !!! UPDATE ONLY WHEN PLAYER REACHED NEW LEVEL OR IS WEARING NEW CLOTHS
	// TODO: DELETE !!! UPDATE ONLY WHEN PLAYER REACHED NEW LEVEL OR IS WEARING NEW CLOTHS
	// TODO: DELETE !!! UPDATE ONLY WHEN PLAYER REACHED NEW LEVEL OR IS WEARING NEW CLOTHS
	CalculateMeleDamage();
	CalculateCriticalChance();
	CalculateManaRegeneration();
	CalculateHealthRegeneration();

	manaPercentage = currentMana / maximumMana;
	healthPercentage = currentHealth / maximumHealth;

	// TODO: MAKE A PROPER ONE
	// Adjust mana and health 
	currentHealth += healthRegeneration * DeltaTime;
	currentMana += manaRegeneration * DeltaTime;
	
	// Check if current mana or health is not higher than current maximum health
	if (currentHealth > maximumHealth) currentHealth = maximumHealth;
	else if (currentHealth < 0) currentHealth = 0;
	if (currentMana > maximumMana) currentMana = maximumMana;
	else if (currentMana < 0) currentMana = 0;
}

/*
// Health attributes getters functions
const inline float UAttributeSystem::GetCurrentHealth()			{ return currentHealth; }
const inline float UAttributeSystem::GetMaximumHealth()			{ return maximumHealth; }
const inline float UAttributeSystem::GetHealthPercentage()		{ return float(currentHealth / maximumHealth); }

// Mana attributes getters functions
const inline float UAttributeSystem::GetCurrentMana()			{ return currentMana; }
const inline float UAttributeSystem::GetMaximumMana()			{ return maximumMana; }
const inline float UAttributeSystem::GetManaPercentage()		{ return float(currentMana / maximumMana); }

// Main attributes getters functions
const inline int UAttributeSystem::GetStrength()				{ return strength; }
const inline int UAttributeSystem::GetAgility()					{ return agility; }
const inline int UAttributeSystem::GetVitality()				{ return vitality; }
const inline int UAttributeSystem::GetIntelligence()			{ return intelligence; }
const inline int UAttributeSystem::GetSpirit()					{ return spirit; }

// Additional attributes getters functions
const inline int UAttributeSystem::GetMinimumAttackPower()		{ return minimumAttackPower; }
const inline int UAttributeSystem::GetMaximumAttackPower()		{ return maximumAttackPower; }
const inline int UAttributeSystem::GetMinimumMagicPower()		{ return minimumMagicPower; }
const inline int UAttributeSystem::GetMaximumMagicPower()		{ return maximumMagicPower; }
const inline float UAttributeSystem::GetHealthRegeneration()	{ return healthRegeneration; }
const inline float UAttributeSystem::GetManaRegeneration()		{ return manaRegeneration; }
const inline float UAttributeSystem::GetCriticalChance()		{ return criticalChance; }


const inline int UAttributeSystem::GetArmor()					{ return armor; }
const inline int UAttributeSystem::GetMagicResistance()			{ return magicResistance; }
*/


void UAttributeSystem::CalculateMeleDamage()
{
	minimumAttackPower = minimumAttackPowerFromWeapon + uint16((strength * 0.5f) - ((strength * 0.5f) * 0.33f));
	maximumAttackPower = maximumAttackPowerFromWeapon + uint16((strength * 0.5f) + ((strength * 0.5f) * 0.33f));

	minimumMagicPower = uint16((intelligence * 0.5f) - ((intelligence * 0.5f) * 0.33f));
	maximumMagicPower = uint16((intelligence * 0.5f) + ((intelligence * 0.5f) * 0.33f));
	// Addjust weapon damage

}

void UAttributeSystem::CalculateCriticalChance()
{
	criticalChance = ((float)agility / AAttributesCalculator::TABLE_AGILITY_TO_CRITICAL_CHANCE[currentLevel]) * 100.0f;
}

void UAttributeSystem::CalculateManaRegeneration()
{
	manaRegeneration = spirit * 0.1f;
}

void UAttributeSystem::CalculateHealthRegeneration()
{
	healthRegeneration = vitality * 0.1f;
}