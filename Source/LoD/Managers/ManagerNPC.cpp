// Fill out your copyright notice in the Description page of Project Settings.

#include "ManagerNPC.h"
#include "References.h"
#include "../Actors/NPC.h"
#include "../LoDCharacter.h"
#include "../Components/WidgetComponent.h"
#include "AI/Navigation/NavigationSystem.h"
#include "Engine.h"
#include "Camera/CameraComponent.h"



#include "../Controllers/AIEnemyController.h"

#define DISTANCE_TO_INTERACT_WITH_NPC_WIDGET 2000000
#define DISTANCE_TO_INTERACT_WITH_NPC_MOVEMENT 1000000
#define CLOSESST_NPC_DISTANCE_TO_PLAYER 30000

// Sets default values for this component's properties
UManagerNPC::UManagerNPC()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called every frame
void UManagerNPC::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (ptrCharacter->currentLocation)
	{
	case ECurrentLocation::LVL2:		UpdateNPC(npcArrayLVL2);			break;
	case ECurrentLocation::LVL3_Part1:	UpdateNPC(npcArrayLVL3_Part1);		break;
	case ECurrentLocation::LVL3_Part2:	UpdateNPC(npcArrayLVL3_Part2);		break;
	case ECurrentLocation::LVL4_Part1:	UpdateNPC(npcArrayLVL4_Part1);		break;
	case ECurrentLocation::LVL5:		UpdateNPC(npcArrayLVL5);			break;
	}
}

void UManagerNPC::UpdateNPC(TArray<ANPC * > & npcArrayToUpdate)
{
	if (npcArrayToUpdate.Num() > 0)
	{
		UNavigationSystem * const NavSys = GetWorld()->GetNavigationSystem();

		for (int32 i = 0; i < npcArrayToUpdate.Num(); i++)
		{
			if (npcArrayToUpdate[i]->IsValidLowLevelFast())
			{
				AAIEnemyController * ptrController = Cast<AAIEnemyController>(npcArrayToUpdate[i]->GetController());
				if (ptrController)
				{
					// Calculate displacement vector
					this->npcDisplacement = ptrCharacter->GetActorLocation() - npcArrayToUpdate[i]->GetActorLocation();
					// Calculate not squared distance
					this->distanceToNPC.Z = this->npcDisplacement.X * this->npcDisplacement.X + this->npcDisplacement.Y * this->npcDisplacement.Y;

					// If distance is negative, multiply it by -1 to get positive number
					if (distanceToNPC.Z < 0.0f) distanceToNPC.Z *= -1.0f;
					if (distanceToNPC.Z < DISTANCE_TO_INTERACT_WITH_NPC_WIDGET)
					{
						// Make widget visible
						npcArrayToUpdate[i]->GetNameWidget()->SetVisibility(true);
						// Rotate towards the screen
						npcArrayToUpdate[i]->GetNameWidget()->SetWorldRotation(FQuat::MakeFromEuler(FVector(0.0f, 40.0f, 180 + ptrCharacter->GetCameraComponent()->GetComponentRotation().Yaw)));

						if (distanceToNPC.Z < DISTANCE_TO_INTERACT_WITH_NPC_MOVEMENT)
						{

							if (NavSys && distanceToNPC.Z > CLOSESST_NPC_DISTANCE_TO_PLAYER)
							{
								// Set Chasing State
								ptrController->behaviourState = EAIBehaviourState::CHASING;
								ptrController->MoveToLocation(ptrCharacter->GetActorLocation(), 10.0f, true);
							}
							else // Set Attacking State
							{							
								ptrController->behaviourState = EAIBehaviourState::ATTACKING;
								ptrController->MoveToLocation(ptrCharacter->GetActorLocation(), 10.0f, true);
							}
						}
						else // Set Wandering State
						{
							ptrController->behaviourState = EAIBehaviourState::WANDERING;
						}
					}
					else // Make widget invisible
					{
						npcArrayToUpdate[i]->GetNameWidget()->SetVisibility(false);
					}
				}
			}
		}
	}
}

/** Push new element into npcNamesArray */
void UManagerNPC::PushNewNPC(ANPC * newNPC)
{
	switch (newNPC->currentLocation)
	{
	case ECurrentLocation::LVL2:		npcArrayLVL2.Push(newNPC);			break;
	case ECurrentLocation::LVL3_Part1:	npcArrayLVL3_Part1.Push(newNPC);	break;
	case ECurrentLocation::LVL3_Part2:	npcArrayLVL3_Part2.Push(newNPC);	break;
	case ECurrentLocation::LVL4_Part1:	npcArrayLVL4_Part1.Push(newNPC);	break;
	case ECurrentLocation::LVL5:		npcArrayLVL5.Push(newNPC);			break;
	}

	/** This prevents AI in locaion without nav mesh to not fall down and not get delete when they get bellow -10 000 */
	if (newNPC->currentLocation == ptrCharacter->currentLocation)
	{
		newNPC->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_NavWalking);
	}
	else newNPC->GetCharacterMovement()->DisableMovement();

}

/** Remove element from npcNamesArray */
void UManagerNPC::RemoveNPC(ANPC * dyingNPC)
{
	switch (dyingNPC->currentLocation)
	{
	case ECurrentLocation::LVL2:		npcArrayLVL2.Remove(dyingNPC);			break;
	case ECurrentLocation::LVL3_Part1:	npcArrayLVL3_Part1.Remove(dyingNPC);	break;
	case ECurrentLocation::LVL3_Part2:	npcArrayLVL3_Part2.Remove(dyingNPC);	break;
	case ECurrentLocation::LVL4_Part1:	npcArrayLVL4_Part1.Remove(dyingNPC);	break;
	case ECurrentLocation::LVL5:		npcArrayLVL5.Remove(dyingNPC);			break;
	}
}

void UManagerNPC::DissableNPCAtLevel(ECurrentLocation atLocation)
{
	switch (atLocation)
	{
	case ECurrentLocation::LVL2:		DisableNPC(npcArrayLVL2);		break;
	case ECurrentLocation::LVL3_Part1:	DisableNPC(npcArrayLVL3_Part1);	break;
	case ECurrentLocation::LVL3_Part2:	DisableNPC(npcArrayLVL3_Part2);	break;
	case ECurrentLocation::LVL4_Part1:	DisableNPC(npcArrayLVL4_Part1);	break;
	case ECurrentLocation::LVL5:		DisableNPC(npcArrayLVL5);		break;
	}
}

void UManagerNPC::EnableNPCAtLevel(ECurrentLocation atLocation)
{
	switch (atLocation)
	{
	case ECurrentLocation::LVL2:		EnableNPC(npcArrayLVL2);		break;
	case ECurrentLocation::LVL3_Part1:	EnableNPC(npcArrayLVL3_Part1);	break;
	case ECurrentLocation::LVL3_Part2:	EnableNPC(npcArrayLVL3_Part2);	break;
	case ECurrentLocation::LVL4_Part1:	EnableNPC(npcArrayLVL4_Part1);	break;
	case ECurrentLocation::LVL5:		EnableNPC(npcArrayLVL5);		break;
	}
}


void UManagerNPC::EnableNPC(TArray<ANPC * > & npcArrayToUpdate)
{
	for (int32 i = 0; i < npcArrayToUpdate.Num(); i++)
	{
		if (npcArrayToUpdate[i]->IsValidLowLevelFast())
		{
			AAIEnemyController * ptrController = Cast<AAIEnemyController>(npcArrayToUpdate[i]->GetController());
			if (ptrController)
			{
				ptrController->EnableMovement();
			}
		}
	}
}

void UManagerNPC::DisableNPC(TArray<ANPC * > & npcArrayToUpdate)
{
	for (int32 i = 0; i < npcArrayToUpdate.Num(); i++)
	{
		if (npcArrayToUpdate[i]->IsValidLowLevelFast())
		{
			AAIEnemyController * ptrController = Cast<AAIEnemyController>(npcArrayToUpdate[i]->GetController());
			if (ptrController)
			{
				ptrController->StopMovement();
			}
		}
	}
}
