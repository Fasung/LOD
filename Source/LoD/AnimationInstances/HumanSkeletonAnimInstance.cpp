// Fill out your copyright notice in the Description page of Project Settings.

#include "HumanSkeletonAnimInstance.h"
#include "../Public/EquipmentSystem.h"
#include "../Public/InventorySystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "LoDCharacter.h"
#include "References.h"
#include "Engine.h"



void UHumanSkeletonAnimInstance::StartDrinkingHealthPotion()
{
	if (!IsDrinkingPotion)
	{
		GetWorld()->GetTimerManager().SetTimer(animationTimer, this, &UHumanSkeletonAnimInstance::StopDrinkingPotion, 3.0f, false);
		
		// REMOVE THIS
		// REMOVE THIS
		// REMOVE THIS

		if (ptrCharacter->GetInventorySystem()->GetHealthPotion())
		{
			ptrCharacter->GetEquipmentSystem()->StartDrinkPotion(ptrCharacter->GetInventorySystem()->GetHealthPotion());
			ptrCharacter->GetInventorySystem()->DecreaseHealthPotionsCount();
		}	

		ptrCharacter->GetCharacterMovement()->DisableMovement();
		IsDrinkingPotion = true;
	}
}

void UHumanSkeletonAnimInstance::StartDrinkingManaPotion()
{
	if (!IsDrinkingPotion)
	{
		GetWorld()->GetTimerManager().SetTimer(animationTimer, this, &UHumanSkeletonAnimInstance::StopDrinkingPotion, 3.0f, false);

		// REMOVE THIS
		// REMOVE THIS
		// REMOVE THIS

		if (ptrCharacter->GetInventorySystem()->GetManaPotion())
		{
			ptrCharacter->GetEquipmentSystem()->StartDrinkPotion(ptrCharacter->GetInventorySystem()->GetManaPotion());
			ptrCharacter->GetInventorySystem()->DecreaseManaPotionsCount();
		}

		ptrCharacter->GetCharacterMovement()->DisableMovement();
		IsDrinkingPotion = true;
	}
}

void UHumanSkeletonAnimInstance::StopDrinkingPotion()
{
	IsDrinkingPotion = false;
	ptrCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	ptrCharacter->GetEquipmentSystem()->StopDrinkingPotion();
}