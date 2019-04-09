// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class LOD_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public: /** Variables */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimationState")
	bool IsMeleeAttacking = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimationState")
	bool IsMoving = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimationState")
	bool IsWaiting = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimationState")
	bool IsDead = false;
	

public: /** Functions */
	
	/** */
	FORCEINLINE void SetToMovingState();

	/** */
	FORCEINLINE void SetToMeleeAttackingState();

	/** */
	FORCEINLINE void SetToWaitingState();

	/** */
	FORCEINLINE void SetToDeadState();
};
