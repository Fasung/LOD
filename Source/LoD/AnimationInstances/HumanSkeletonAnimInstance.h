// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Engine.h"
#include "Animation/AnimInstance.h"
#include "HumanSkeletonAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class LOD_API UHumanSkeletonAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CombatAnimations")
	bool IsMeleeAttacking;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OtherAnimations")
	bool IsDrinkingPotion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimationState")
	bool CanApplyDamage = false;

	FTimerHandle animationTimer;

public:
	UFUNCTION()	void StartDrinkingHealthPotion();
	
	UFUNCTION()	void StartDrinkingManaPotion();

private:
	UFUNCTION()	void StopDrinkingPotion();

};
