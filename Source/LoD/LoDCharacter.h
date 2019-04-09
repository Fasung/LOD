// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.


#pragma once
#include "CoreMinimal.h"
#include "LoDEnumTypes.h"
#include "GameFramework/Character.h"
#include "LoDCharacter.generated.h"


UCLASS(Blueprintable)
class ALoDCharacter : public ACharacter
{
	GENERATED_BODY()
		
private: /** Variables */

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent * cameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent * cameraBoom;

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent * cursorToWorld;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterSystems", meta = (AllowPrivateAccess = "true"))
	class UAttributeSystem * attributeSystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterSystems", meta = (AllowPrivateAccess = "true"))
	class UInventorySystem * inventorySystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterSystems", meta = (AllowPrivateAccess = "true"))
	class UHumanSkeletonAnimInstance * animationSystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterSystems", meta = (AllowPrivateAccess = "true"))
	class UEquipmentSystem * equipmentSystem;

	UPROPERTY(VisibleAnywhere, Category = "CharacterSystems", meta = (AllowPrivateAccess = "true"))
	class UTargetingSystem * targetingSystem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterSystems", meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent * PSSSS_levelUp;

	/** LevelUP audio effect, called when player reached new level */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Other_Details", meta = (AllowPrivateAccess = "true"))
	USoundBase * SE_LevelUP; 

public: /** Variables */

	/** Determine in which location is NPC at the moment */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Other_Details")
	ECurrentLocation currentLocation;

protected: /** Functions */
	/** Called every frame */
	virtual void Tick(float DeltaSeconds) override;

	/** Called before the game start*/
	virtual void BeginPlay() override;

public:

	FORCEINLINE	class UCameraComponent * GetCameraComponent() const				{ return this->cameraComponent; }

	FORCEINLINE	class USpringArmComponent * GetCameraBoom() const				{ return this->cameraBoom; }

	FORCEINLINE	class UDecalComponent * GetCursorToWorld() const				{ return this->cursorToWorld; }

	FORCEINLINE	class UAttributeSystem * GetAttributeSystem() const				{ return this->attributeSystem; }

	FORCEINLINE	class UInventorySystem * GetInventorySystem() const				{ return this->inventorySystem; }

	FORCEINLINE	class UHumanSkeletonAnimInstance * GetAnimationSystem() const	{ return this->animationSystem; }

	FORCEINLINE	class UEquipmentSystem * GetEquipmentSystem() const				{ return this->equipmentSystem; }

	FORCEINLINE	class UTargetingSystem * GetTargetingSystem() const				{ return this->targetingSystem; }

public: /** Functions */

	UFUNCTION() void PlaySoundEffectLevelUP();

	/** Disable character movement, called from LoDPlayerController, usually when player drinks potions or using abilities */
//	UFUNCTION() void DisableCharacterMovement();

	/** Enable character movement, called from LoDPlayerController, usually after player used abilities or drunk potion */
//	UFUNCTION() void EnableCharacterMovement();

public: /** Constructors */
	ALoDCharacter();
};

