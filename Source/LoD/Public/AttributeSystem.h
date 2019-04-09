// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributesCalculator.h"
#include "AttributeSystem.generated.h"

/*
THERE IS A LOT OF COMMENTED FUNCTIONS CHECH WHAT I SHOULD DO WITH THEM :D
THERE IS A LOT OF COMMENTED FUNCTIONS CHECH WHAT I SHOULD DO WITH THEM :D
THERE IS A LOT OF COMMENTED FUNCTIONS CHECH WHAT I SHOULD DO WITH THEM :D
THERE IS A LOT OF COMMENTED FUNCTIONS CHECH WHAT I SHOULD DO WITH THEM :D
THERE IS A LOT OF COMMENTED FUNCTIONS CHECH WHAT I SHOULD DO WITH THEM :D
THERE IS A LOT OF COMMENTED FUNCTIONS CHECH WHAT I SHOULD DO WITH THEM :D
THERE IS A LOT OF COMMENTED FUNCTIONS CHECH WHAT I SHOULD DO WITH THEM :D
*/


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LOD_API UAttributeSystem : public UActorComponent
{
	GENERATED_BODY()
public:

	// Health attributes
	UPROPERTY(BlueprintReadWrite, Category = "Attributes" )
	float currentHealth;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attributes" )
	float maximumHealth;
	UPROPERTY(BlueprintReadWrite, Category = "Attributes" )
	float healthPercentage;


	// Mana attributes
	UPROPERTY(BlueprintReadWrite, Category = "Attributes" )
	float currentMana;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attributes" )
	float maximumMana;
	UPROPERTY(BlueprintReadWrite, Category = "Attributes" )
	float manaPercentage;


	// Main attributes
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attributes" )
	int strength;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attributes" )
	int agility;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attributes" )
	int vitality;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attributes" )
	int intelligence;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attributes" )
	int spirit;


	// Defensive Attributes - they are addjusted only from equipment
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attributes" )
	int armor;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attributes" )
	int magicResistance;


	// Additional Attributes - these attributes are calculated from main attributes
	int minimumAttackPowerFromWeapon;
	int maximumAttackPowerFromWeapon;;
	UPROPERTY(BlueprintReadWrite, Category = "Attributes" )
	int minimumAttackPower;
	UPROPERTY(BlueprintReadWrite, Category = "Attributes" )
	int maximumAttackPower;
	UPROPERTY(BlueprintReadWrite, Category = "Attributes" )
	int minimumMagicPower;
	UPROPERTY(BlueprintReadWrite, Category = "Attributes" )
	int maximumMagicPower;
	UPROPERTY(BlueprintReadWrite, Category = "Attributes" )
	float healthRegeneration;
	UPROPERTY(BlueprintReadWrite, Category = "Attributes" )
	float manaRegeneration;
	UPROPERTY(BlueprintReadWrite, Category = "Attributes" )
	float criticalChance;


	// Level attributes
	UPROPERTY(BlueprintReadWrite, Category = "Attributes" )
	int currentExperience;
	UPROPERTY(BlueprintReadWrite, Category = "Attributes" )
	int experienceToNextLevel;
	UPROPERTY(BlueprintReadWrite, Category = "Attributes" )
	int currentLevel;

	
private:

	class UParticleSystemComponent * PS_levelUpEffect;


public:	// Virtual Functions
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Called before the game start*/
	virtual void BeginPlay() override;

public: // Functions

	/** Increase character experience by obtained parameter, use when player complete quest or kill enemy,
	function also check if is reached new level or if is current experience not bigger than experience needed to achieve higher level */
	UFUNCTION() void IncreaseExperience(int addjustExperience);

	const FORCEINLINE int GetLevel()					{ return this->currentLevel; }

	const FORCEINLINE void AttachLevelUpParticleSystem(class UParticleSystemComponent * newParticleSystem)		{ this->PS_levelUpEffect = newParticleSystem; }

	FORCEINLINE void DissableAttributeSystem()				{ PrimaryComponentTick.SetTickFunctionEnable(false); }

	FORCEINLINE void EnableAttributeSystem()				{ PrimaryComponentTick.SetTickFunctionEnable(true); }

private:

	/** Used only for blueprint widget, used for showing up health in intigers instead of floats */
	UFUNCTION(BlueprintCallable, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	int GetCurrentHealthIntiger()				{ return static_cast<int>(this->currentHealth); }

	/** Used only for blueprint widget, used for showing up health in intigers instead of floats */
	UFUNCTION(BlueprintCallable, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	int GetMaximalHealthIntiger()				{ return static_cast<int>(this->maximumHealth); }

	/** Used only for blueprint widget, used for showing up health in intigers instead of floats */
	UFUNCTION(BlueprintCallable, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	int GetCurrentManaIntiger()					{ return static_cast<int>(this->currentMana); }

	/** Used only for blueprint widget, used for showing up health in intigers instead of floats */
	UFUNCTION(BlueprintCallable, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	int GetMaximalManaIntiger()					{ return static_cast<int>(this->maximumMana); }

	/*
	// Health attributes getters
	const inline float GetCurrentHealth();
	const inline float GetMaximumHealth();
	const inline float GetHealthPercentage();

	// Mana attributes getters
	const inline float GetCurrentMana();
	const inline float GetMaximumMana();
	const inline float GetManaPercentage();

	// Main attributes getters
	const inline int GetStrength();
	const inline int GetAgility();
	const inline int GetVitality();
	const inline int GetIntelligence();
	const inline int GetSpirit();

	// Additional attributes getters
	const inline int GetMinimumAttackPower();
	const inline int GetMaximumAttackPower();
	const inline int GetMinimumMagicPower();
	const inline int GetMaximumMagicPower();
	const inline float GetHealthRegeneration();
	const inline float GetManaRegeneration();
	const inline float GetCriticalChance();
	const inline int GetArmor();
	const inline int GetMagicResistance();
	*/

private: // Functions
	void CalculateMeleDamage();
	void CalculateCriticalChance();
	void CalculateManaRegeneration();
	void CalculateHealthRegeneration();
		
public:	// Constructors
		// Sets default values for this component's properties
	UAttributeSystem();

};
