// Fill out your copyright notice in the Description page of Project Settings.

#include "TargetingSystemNPC.h"
#include "../Public/AttributeSystem.h"
#include "../Actors/NPC.h"
#include "LoDCharacter.h"
#include "LoDHUD.h"
#include "References.h"

// Sets default values for this component's properties
UTargetingSystemNPC::UTargetingSystemNPC()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTargetingSystemNPC::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTargetingSystemNPC::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTargetingSystemNPC::ApplyDamage()
{
	
	if (target != NULL)
	{
		float tempCriticalDamage = FMath::RandRange(0, 100);

		ANPC * owningPawn = Cast<ANPC>(GetOwner());

		if (owningPawn)
		{
			float damage = FMath::RandRange(owningPawn->GetAttributeSystem()->minimumAttackPower, owningPawn->GetAttributeSystem()->maximumAttackPower);
			if (tempCriticalDamage <= owningPawn->GetAttributeSystem()->criticalChance)
			{
				damage *= 2.5f;
				ptrHUD->SpawnFloatingTextWidget(target, EFloatingTextType::DAMAGE_DONE_ENEMY, damage, true);
			}
			else
			{
				ptrHUD->SpawnFloatingTextWidget(target, EFloatingTextType::DAMAGE_DONE_ENEMY, damage, false);
			}

			target->GetAttributeSystem()->currentHealth -= damage;
		}
	}
}