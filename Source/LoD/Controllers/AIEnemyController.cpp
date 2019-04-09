// Fill out your copyright notice in the Description page of Project Settings.

#include "AIEnemyController.h"
#include "AI/Navigation/NavigationSystem.h"
#include "Runtime/AIModule/Classes/BrainComponent.h"
#include "../AnimationInstances/EnemyAnimInstance.h"
//#include "../Components/TargetingSystemNPC.h"
#include "../Actors/NPC.h"
#include "../Public/References.h"
#include "../LoDCharacter.h"
#include "Engine.h"

void AAIEnemyController::DoAction()
{
	if (this->animationSystem)
	{
		if (this->navigationSystem && this->NPC->GetCharacterMovement()->MovementMode != EMovementMode::MOVE_None)
		{
			switch (behaviourState)
			{
			case EAIBehaviourState::WANDERING:
				this->CanPlayAgroSoundEffect = true;
				this->movingDestination = this->navigationSystem->GetRandomPointInNavigableRadius(this->NPC, this->spawnPosition, 300.0f);
				MoveToLocation(this->movingDestination, 10.0f, true);
				this->animationSystem->SetToMovingState();
				NPC->GetTargetingSystem()->ClearTarget();
				break;

			case EAIBehaviourState::IDLE:
				this->animationSystem->SetToWaitingState();
				this->behaviourState = EAIBehaviourState::WANDERING;
				GetWorld()->GetTimerManager().SetTimer(this->stateTimer, this, &AAIEnemyController::DoAction, FMath::RandRange(2, 10), false);

				break;

			case EAIBehaviourState::CHASING:
				if (this->CanPlayAgroSoundEffect)
				{
					this->NPC->PlayAgroSoundEffect();
					this->CanPlayAgroSoundEffect = false;
				}
				
				NPC->GetTargetingSystem()->SetTarget(ptrCharacter);
				MoveToLocation(ptrCharacter->GetActorLocation(), 10.0f, true);
				this->animationSystem->SetToMovingState();
				break;

			case EAIBehaviourState::ATTACKING:
				MoveToLocation(ptrCharacter->GetActorLocation(), 10.0f, true);
				this->animationSystem->SetToMeleeAttackingState();
				break;

			case EAIBehaviourState::DEAD:
				this->animationSystem->SetToDeadState();
				this->NPC->GetCharacterMovement()->DisableMovement();
				this->NPC->GetCharacterMovement()->StopMovementImmediately();

				break;
			}
		}
	}
}

void AAIEnemyController::StopMovement()
{
	this->NPC->GetCharacterMovement()->DisableMovement();
}

void AAIEnemyController::EnableMovement()
{
	this->NPC->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_NavWalking);
	this->DoAction();
}


void AAIEnemyController::OnMoveCompleted(FAIRequestID requestID, const FPathFollowingResult & result)
{
	if (this->NPC->GetCharacterMovement()->MovementMode != EMovementMode::MOVE_None)
	{
		if (this->behaviourState == EAIBehaviourState::WANDERING)
		{
			this->behaviourState = EAIBehaviourState::IDLE;
		}

		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("ACTION COMPLETED - try to do new action")));
		this->DoAction();
	}
}

void AAIEnemyController::InitializeControllerVariables(ANPC * npc, EAIBehaviourState newBehaviourState)
{
	this->NPC = npc;
	this->CanPlayAgroSoundEffect = true;
	this->spawnPosition = npc->GetActorLocation();
	this->navigationSystem = GetWorld()->GetNavigationSystem();
	this->animationSystem = dynamic_cast<UEnemyAnimInstance*>(npc->GetMesh()->GetAnimInstance());
	this->behaviourState = newBehaviourState;
	this->DoAction();
}
