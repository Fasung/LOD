// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TargetingSystem.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LOD_API UTargetingSystem : public UActorComponent
{
	GENERATED_BODY()

private:
	/** Boolean logic which teremain whenever is something targeted or not */
	uint32 bHasTarget : 1;

	/** Boolean logic which teremain whenever is something targeted or not */
	uint32 bHasTargetNPC : 1;

	/** Boolean logic which teremain whenever is targeted item static or not */
	uint32 bIsTargetStatic : 1;

	/** Used to calculate target distance, coordinate X,Y keep target horizontal displacement, coordinate Y store not squared distance */
	UPROPERTY() FVector m_targetDistance;

	/** Used to calculate target distance, this parameter is obtain in function call SetTarget */
	UPROPERTY() FVector m_targetPosition;

	/** Store the pointer to the last clicked ACtor with OnClickEvent */
	UPROPERTY() class AActor * target;

	/** Store the pointer to the last clicked NPC with OnClickEvent */
	UPROPERTY() class ANPC * npcTarget;

	/** Store the type of the target */
	enum ETargetType m_targetType;

	/** Store the type of the target */
	enum ENPC_Type m_typeNPC;

public:
	// Sets default values for this component's properties
	UTargetingSystem();

public: /** Utility functions */

	/** Called when player has target, update targets and interact with them */
	UFUNCTION() void UpdateTargetingSystem();

	/** Set new target */
	UFUNCTION()	void SetTarget(AActor * newTarget, enum ETargetType newTargetType);

	/** Set new NPC target */
	UFUNCTION()	void SetNPCTarget(ANPC * newTarget, enum ETargetType newTargetType, enum ENPC_Type typeNPC);

	/** Set new static target */
	UFUNCTION()	void SetStaticTarget(AActor * newTarget, enum ETargetType newTargetType, FVector newStaticActorPosition);


public: /** FORCEINLINE functions */
	/** Return target state */
	const FORCEINLINE bool HasTarget() const			{ return bHasTarget; }

	/** Return target state */
	const FORCEINLINE bool HasTargetNPC() const			{ return bHasTargetNPC; }

	/** Return target */
	const FORCEINLINE class AActor * GetTarget() const	{ return target; }

	/** Return NPC target */
	FORCEINLINE class ANPC * GetTargetNPC() const		{ return npcTarget; }

	/** Clear target socket */
	FORCEINLINE void ClearTarget()						{ bHasTarget = false; bHasTargetNPC = false; target = NULL; }
};
