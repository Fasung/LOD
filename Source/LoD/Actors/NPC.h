// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LoDEnumTypes.h"
#include "NPC.generated.h"

UCLASS()
class LOD_API ANPC : public ACharacter
{
	GENERATED_BODY()

private: /** Properties */

	//UPROPERTY()
	//class AAIEnemyController * controller;

	/** The main skeletal mesh associated with this NPC */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPC_Details", meta = (AllowPrivateAccess = "true"))
	class UTargetingSystemNPC * targetingSystem;

	/** Determine whhat type the NPC is, could be enemy, friendly or neutral */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPC_Details", meta = (AllowPrivateAccess = "true"))
	ENPC_Type npcType;

	/** Store NPC name, name is displayed above the NPC and in NPC dialogs */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPC_Details", meta = (AllowPrivateAccess = "true"))
	FString npcName;
		
	/** Timer is used to destroy NPW with delay */
	FTimerHandle timer;

	/** Store pointer to blueprint widget, blueprint widget containt 3D transformation in the world and text block which is showed 
	*	above the player. On begin play widget is pushed into ALoDHUD's array which maintain widget transformations and visibility */
	UPROPERTY()	class UWidgetComponent * nameWidget;

	/** Store texture which is displayed when is player speaking with NPC */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC_Details", meta = (AllowPrivateAccess = "true"))
	class UTexture2D * npcVisualTexture;

	/** Attribute system, store NPC's stats */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC_Details", meta = (AllowPrivateAccess = "true"))
	class UAttributeSystem * attributeSystem;

private: /** Audio Properties */

	/** Agro audio effect, called when enemies has got new target */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC_Details", meta = (AllowPrivateAccess = "true"))
	USoundBase * SE_Agro_01;

public:  /** Properties */

	/** Determine in which location is NPC at the moment */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPC_Details")
	ECurrentLocation currentLocation;

protected:

	virtual void BeginPlay() override;

private:	/** Functions - Binded dynamics */
	/** Outline NPC on mouse hover */
	UFUNCTION()	void OutlineNPC(UPrimitiveComponent * TouchedComponent);

	/** Do not outline NPC on mouse hover ended */
	UFUNCTION()	void CancelOutlinegNPC(UPrimitiveComponent * TouchedComponent);

	/** Set pawn into targeting system */
	UFUNCTION()	void OnClickSetToTargetingSystem(AActor * ClickedComp, FKey ButtonPressed);

public: /** Getters Functions */

	/** Returns Mesh subobject **/
	//class USkeletalMeshComponent* GetMesh()	const { return this->meshNPC; }

	/** Return NPC type, could be enemy, friendly, SHOP */
	FORCEINLINE ENPC_Type GetType()	{ return this->npcType; }

	/** Return name widget, name widget contain health bar and name of NPC which is displayed above characater */
	FORCEINLINE class UWidgetComponent * GetNameWidget() const { return this->nameWidget; }

	/** Return targeting system */
	FORCEINLINE class UTargetingSystemNPC * GetTargetingSystem() const { return this->targetingSystem; }

	/** Return attribute system */
	FORCEINLINE class UAttributeSystem * GetAttributeSystem() const { return this->attributeSystem; }

	/** Return visual NPC texture, texture is showed when player speak with NPC */
	UFUNCTION() class UTexture2D * GetVisualTexture() const;

	/** Play agro sound effect */
	FORCEINLINE void PlayAgroSoundEffect();

	/**  */
	UFUNCTION() void KillNPC();

	/** */
	UFUNCTION() void DestroyNPC()			{ Destroy(); }

public:	/** Constructors */ 
	/** Sets default values for this pawn's properties */
	ANPC();
};
