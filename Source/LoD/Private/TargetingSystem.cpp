// Fill out your copyright notice in the Description page of Project Settings.

#include "TargetingSystem.h"
#include "AttributeSystem.h"
#include "InventorySystem.h"
#include "References.h"
#include "Item.h"
#include "TreasureChest.h"
#include "TreasureDestructible.h"
#include "../AnimationInstances/HumanSkeletonAnimInstance.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "../Actors/NPC.h"
#include "../Actors/NPC_Shop.h"
#include "../Actors/Elevator.h"
#include "../LoDCharacter.h"
#include "../LoDHUD.h"
#include "../LoDPlayerController.h"
#include "../Components/ShopSystem.h"
#include "../LoDEnumTypes.h"
#include "Engine.h"


// Sets default values for this component's properties
UTargetingSystem::UTargetingSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	bHasTarget = false;
	bHasTargetNPC = false;
	bIsTargetStatic = false;
	target = NULL;
	npcTarget = NULL;
}

/** Called when player has target */
void UTargetingSystem::UpdateTargetingSystem()
{
	// If has target
	if (target)
	{
		// Calculate displacement, if target is static use given parameters, otherwise get paramateres from actor
		// Static parameters might be used for actors which containt more meshesh, and collisions check is needed with specific point
		if (bIsTargetStatic) m_targetDistance = m_targetPosition - ptrCharacter->GetActorLocation();
		else m_targetDistance = target->GetActorLocation() - ptrCharacter->GetActorLocation();
		// Calculate not squared distance
		m_targetDistance.Z = m_targetDistance.X * m_targetDistance.X + m_targetDistance.Y * m_targetDistance.Y;
		// If distance is negative, multiply it by -1 to get positive number
		if (m_targetDistance.Z < 0.0f) m_targetDistance.Z *= -1.0f;

		if (m_targetDistance.Z < 35000.0f)
		{
			switch (m_targetType)
			{
			case ETargetType::TREASURE_CHEST:	// Open the treasure chest
			{
				dynamic_cast<ATreasureChest*>(target)->OpenChest();
				bHasTarget = false;
				return;
			}
			case ETargetType::TREASURE_DESTRUCTIBLE:	// Break the treasure structure mesh
			{
				ptrCharacter->GetAnimationSystem()->IsMeleeAttacking = true;

				FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(ptrCharacter->GetActorLocation(), target->GetActorLocation());
				newRotation.Roll = 0;
				newRotation.Pitch = 0;
				ptrCharacter->SetActorRotation(newRotation);

				if (ptrCharacter->GetAnimationSystem()->CanApplyDamage)
				{
					dynamic_cast<ATreasureDestructible*>(target)->BreakObject();
					ptrCharacter->GetAnimationSystem()->IsMeleeAttacking = false;
					ptrCharacter->GetAnimationSystem()->CanApplyDamage = false;
					bHasTarget = false;
					target = NULL;
				}
				return;
			}
			case ETargetType::ITEM:
			{
				//target->OnTargetingSystemInteraction();
				dynamic_cast<AItem*>(target)->PickUpItem();
				bHasTarget = false;
				return;
			}
			case ETargetType::ELEVATOR:
			{
				dynamic_cast<AElevator*>(target)->MoveElevator();
				bHasTarget = false;
				return;
			}
			case ETargetType::NPC:
			{
				switch (this->m_typeNPC)
				{
				case ENPC_Type::ENEMY:
				{
					if (npcTarget)
					{
						if (ptrCharacter->GetAnimationSystem()->CanApplyDamage)
						{
							ptrCharacter->GetAnimationSystem()->CanApplyDamage = false;
							float tempCriticalDamage = FMath::RandRange(0, 100);

							float damage = FMath::RandRange(ptrCharacter->GetAttributeSystem()->minimumAttackPower, ptrCharacter->GetAttributeSystem()->maximumAttackPower);
							if (tempCriticalDamage <= ptrCharacter->GetAttributeSystem()->criticalChance)
							{
								damage *= 2.5f;
								ptrHUD->SpawnFloatingTextWidget(npcTarget, EFloatingTextType::DAMAGE_DONE, damage, true);
							}
							else
							{
								ptrHUD->SpawnFloatingTextWidget(npcTarget, EFloatingTextType::DAMAGE_DONE, damage, false);
							}
							npcTarget->GetAttributeSystem()->currentHealth -= damage;

							if (npcTarget->GetAttributeSystem()->currentHealth < 1.0f)
							{
								npcTarget->GetAttributeSystem()->healthPercentage = 0.0f;
								npcTarget->GetAttributeSystem()->DissableAttributeSystem();
								npcTarget->KillNPC();
								bHasTarget = false;
								bHasTargetNPC = false;
								npcTarget = false;
								ptrCharacter->GetAnimationSystem()->IsMeleeAttacking = false;
							}
						}
					}
					return;
				}
				case ENPC_Type::FRIENDLY:
				{
					bHasTarget = false;
					return;
				}
				case ENPC_Type::SHOP:
				{
					ANPC_Shop * tempShopTarget = dynamic_cast<ANPC_Shop*>(npcTarget);
					if (tempShopTarget)
					{
						ptrCharacter->GetInventorySystem()->OpenInventory();
						ptrPlayerController->shopSystem->OpenShop();
						ptrPlayerController->shopSystem->shopPosition = this->target->GetActorLocation();
						ptrPlayerController->shopSystem->shopItems = tempShopTarget->GetShopItems();
						ptrPlayerController->shopSystem->ApplyTextureFromNPC(tempShopTarget->GetVisualTexture());
						ptrPlayerController->shopSystem->ApplyFirstAvailableShopThread();
					}
					bHasTarget = false;
					bHasTargetNPC = false;
					npcTarget = false;
					return;
				}
				}
			}
			}
		}
	}
}

/** Set new target*/
void UTargetingSystem::SetTarget(AActor * newTarget, ETargetType newTargetType)
{
	bHasTarget = true;
	bHasTargetNPC = false;
	bIsTargetStatic = false;
	target = newTarget;
	npcTarget = NULL;
	m_targetType = newTargetType;
}

/** Set new target*/
void UTargetingSystem::SetNPCTarget(ANPC * newTarget, ETargetType newTargetType, ENPC_Type newTypeNPC)
{
	bHasTarget = true;
	bHasTargetNPC = true;
	bIsTargetStatic = false;
	target = newTarget;
	npcTarget = newTarget;
	m_targetType = newTargetType;
	m_typeNPC = newTypeNPC;
}

/** Set static target type*/
void UTargetingSystem::SetStaticTarget(AActor * newTarget, ETargetType newTargetType, FVector newStaticActorPosition)
{
	bHasTarget = true;
	bHasTargetNPC = false;
	bIsTargetStatic = true;
	target = newTarget;
	npcTarget = NULL;
	m_targetType = newTargetType;
	m_targetPosition = newStaticActorPosition;
}
