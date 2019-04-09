// Fill out your copyright notice in the Description page of Project Settings.

#include "TreasureDestructible.h"
#include "../LoDEnumTypes.h"
#include "../LoDCharacter.h"
#include "../LoDGameMode.h"
#include "../Public/TargetingSystem.h"
#include "Components/StaticMeshComponent.h"
#include "References.h"
#include "ItemSpawner.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "Runtime/Engine/Classes/Components/AudioComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Engine.h"


// Sets default values
ATreasureDestructible::ATreasureDestructible()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	PS_DestructibleEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PS_BreakEffect"));
	PS_DestructibleEffect->SetupAttachment(Root);

	audioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("audioComponent"));
	
	// Do not play sound effect when it is spawned
	audioComponent->bAutoActivate = false;
}

// Called when the game starts or when spawned
void ATreasureDestructible::BeginPlay()
{
	Super::BeginPlay();
	this->PS_DestructibleEffect->Deactivate();

	// Attach our sound cue to the SoundComponent
	if (SC_BreakDestructible->IsValidLowLevelFast())
	{
		audioComponent->SetSound(SC_BreakDestructible);
	}

	/** Called when the mouse cursor is moved over this actor if mouse over events are enabled in the player controller. */
	Mesh->OnBeginCursorOver.AddDynamic(this, &ATreasureDestructible::OutlineObject);

	/** Called when the mouse cursor is moved off this actor if mouse over events are enabled in the player controller. */
	Mesh->OnEndCursorOver.AddDynamic(this, &ATreasureDestructible::CancelOutlineObject);

	/** Bind the mouse event, on mouse click, set the destructible to targeting system, targeting system decide what to do next */
	OnReleased.AddDynamic(this, &ATreasureDestructible::AddObjectIntoTargetingSystem);
}

void ATreasureDestructible::BreakObject()
{
	if (PS_DestructibleEffect) PS_DestructibleEffect->ActivateSystem();
	if (this->audioComponent) this->audioComponent->Play();
	this->Mesh->SetVisibility(false);
	this->Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	uint8 numberOfItems = FMath::RandRange(0, 11);
	
	if (numberOfItems < 5) numberOfItems = 0;
	else if (numberOfItems < 8) numberOfItems = 1;
	else if (numberOfItems < 10) numberOfItems = 2;
	else if (numberOfItems < 20) numberOfItems = 3;

	for (uint8 i = 0; i < numberOfItems; i++)
	{
		ptrGameMode->itemSpawner->SpawnItem((this->GetActorLocation() + FMath::RandRange(0, 70)), FRotator(0.0f, FMath::RandRange(0.0f, 360.0f), 0.0f));
	}

	GetWorld()->GetTimerManager().SetTimer(this->destroyObjectTimer, this, &ATreasureDestructible::DestroyObject, 3.0f, false);
	RemoveDynamics();
}

void ATreasureDestructible::DestroyObject()
{
	Destroy();
}

void ATreasureDestructible::AddObjectIntoTargetingSystem(AActor * TouchedActor, FKey ButtonPressed)
{
	ptrCharacter->GetTargetingSystem()->SetTarget(this, ETargetType::TREASURE_DESTRUCTIBLE);
}

void ATreasureDestructible::OutlineObject(UPrimitiveComponent * TouchedComponent)
{
	Mesh->SetRenderCustomDepth(true);
}

void ATreasureDestructible::CancelOutlineObject(UPrimitiveComponent * TouchedComponent)
{
	Mesh->SetRenderCustomDepth(false);
}

void ATreasureDestructible::AddDynamics()
{
	/** Called when the mouse cursor is moved over this actor if mouse over events are enabled in the player controller */
	Mesh->OnBeginCursorOver.AddDynamic(this, &ATreasureDestructible::OutlineObject);

	/** Called when the mouse cursor is moved off this actor if mouse over events are enabled in the player controller */
	Mesh->OnEndCursorOver.AddDynamic(this, &ATreasureDestructible::CancelOutlineObject);

	/** Bind the mouse event, on mouse click, set the destructible to targeting system, targeting system decide what to do next */
	OnReleased.AddDynamic(this, &ATreasureDestructible::AddObjectIntoTargetingSystem);
}

void ATreasureDestructible::RemoveDynamics()
{
	/** Unsubscribe all binded events from the object as the object is going to be destroyed or unloaded from scene */
	Mesh->OnBeginCursorOver.RemoveDynamic(this, &ATreasureDestructible::OutlineObject);
	Mesh->OnEndCursorOver.RemoveDynamic(this, &ATreasureDestructible::OutlineObject);
	OnReleased.RemoveDynamic(this, &ATreasureDestructible::AddObjectIntoTargetingSystem);
}
