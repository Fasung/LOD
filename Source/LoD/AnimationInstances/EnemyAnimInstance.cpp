// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAnimInstance.h"



void UEnemyAnimInstance::SetToDeadState()
{
	IsDead = true;
	IsMoving = false;
	IsMeleeAttacking = false;
	IsWaiting = false;
}

void UEnemyAnimInstance::SetToMovingState()
{
	IsMoving = true;
	IsMeleeAttacking = false;
	IsWaiting = false;
}

void UEnemyAnimInstance::SetToMeleeAttackingState()
{
	IsMeleeAttacking = true;
	IsMoving = false;
	IsWaiting = false;
}

void UEnemyAnimInstance::SetToWaitingState()
{
	IsWaiting = true;
	IsMeleeAttacking = false;
	IsMoving = false;
}