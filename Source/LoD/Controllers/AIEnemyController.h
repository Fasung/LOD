// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "../LoDEnumTypes.h"
#include "AIEnemyController.generated.h"

/**
 * 
 */
UCLASS()
class LOD_API AAIEnemyController : public AAIController
{
	GENERATED_BODY()
	
private: /** Variables */
	
	 /** */
	UPROPERTY()	FVector spawnPosition;

	/** */
	UPROPERTY()	FVector movingDestination;

	/** */
	UPROPERTY()	class ANPC * NPC;

	/** */
	UPROPERTY()	class UNavigationSystem * navigationSystem;

	/** */
	UPROPERTY()	class UEnemyAnimInstance * animationSystem;

	/** */
	UPROPERTY()	bool CanPlayAgroSoundEffect;

	/** State timer is used to */
	FTimerHandle stateTimer;

public: /** Variables */

	/** */
	UPROPERTY()	EAIBehaviourState behaviourState;

public: /** Virtual Functions */

	/** */
	virtual void StopMovement() override;

	/** */
	virtual void OnMoveCompleted(FAIRequestID requestID, const FPathFollowingResult & result) override;

public: /** Functions */

	/** */
	UFUNCTION() void InitializeControllerVariables(class ANPC * newNPC, EAIBehaviourState newBehaviourState);

	/** */
	FORCEINLINE void EnableMovement();

	/** */
	FORCEINLINE	class UEnemyAnimInstance * GetAnimationInstance() const		{ return this->animationSystem; }
	
	/** */
	UFUNCTION() void DoAction();

};
