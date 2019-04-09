// Fill out your copyright notice in the Description page of Project Settings.

#include "Item.h"
#include "../Public/InventorySystem.h"
#include "../Public/TargetingSystem.h"
#include "Kismet/GameplayStatics.h"
#include "References.h"
#include "Engine.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "../UserInterface/ItemName.h"
#include "Internationalization/Text.h"
#include "../LoDCharacter.h"


// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	/** Called when the mouse cursor is moved over this actor if mouse over events are enabled in the player controller. */
	Mesh->OnBeginCursorOver.AddDynamic(this, &AItem::OutlineObject);
	MeshAdditional->OnBeginCursorOver.AddDynamic(this, &AItem::OutlineObject);
	/** Called when the mouse cursor is moved off this actor if mouse over events are enabled in the player controller. */
	Mesh->OnEndCursorOver.AddDynamic(this, &AItem::CancelOutlineObject);
	MeshAdditional->OnEndCursorOver.AddDynamic(this, &AItem::CancelOutlineObject);

	/** Bind the mouse event, on mouse click, set the pick up itemSetItemToPickUp to character targeted item */
	OnReleased.AddDynamic(this, &AItem::SetItemToPickUp);

	/** If item property type is money type, generate value and addjust name text */
	switch (itemPropertyType)
	{
	case EItemPropertyType::MONEY_GOLD:
		valueGolds = FMath::RandRange(1, 5);
		name = name + " " + FString::FromInt(valueGolds);
		break;
	case EItemPropertyType::MONEY_SILVER:
		valueSilvers = FMath::RandRange(0, 99);
		name = name + " " + FString::FromInt(valueSilvers);
		break;
	case EItemPropertyType::MONEY_BRONZE:
		valueBronzes = FMath::RandRange(0, 99);
		name = name + " " + FString::FromInt(valueBronzes);
		break;
	}


	itemNameWidget->SetDrawSize(FVector2D((name.Len() * 9), 20.0f));
	itemNameWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	itemNameWidget->SetVisibility(true);
	
	UItemName * newItemWidget = Cast<UItemName>(itemNameWidget->GetUserWidgetObject());
	if (newItemWidget) newItemWidget->SetUpWidget(this->name, this->itemType);
}


// Sets default values
AItem::AItem()
{
	name = "Undefiend";

	// Set this actor to do not call Tick() every frame. 
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	Mesh->CastShadow = false;
	Mesh->bCastStaticShadow = false;
	Mesh->bCastDynamicShadow = false;

	MeshAdditional = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshAdditional"));
	MeshAdditional->SetupAttachment(Root);
	MeshAdditional->CastShadow = false;
	MeshAdditional->bCastStaticShadow = false;
	MeshAdditional->bCastDynamicShadow = false;

	/** Initialize widget component, widget component store item name */
	static ConstructorHelpers::FClassFinder<UUserWidget> hudWidgetObj(TEXT("/Game/HUD/BP_ItemName"));
	itemNameWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Border"));
	itemNameWidget->SetWidgetSpace(EWidgetSpace::Screen);
	itemNameWidget->SetWidgetClass(hudWidgetObj.Class);
	itemNameWidget->SetupAttachment(Root);
}

/*
void AItem::OnTargetingSystemInteraction()
{
	if (ptrCharacter->InventorySystem->PickUpItem(this))
	{
		// Dissable object visiblity
		Mesh->SetVisibility(false);
		MeshAdditional->SetVisibility(false);
		itemNameWidget->SetVisibility(false);
		// Dissable collision with mouse
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		MeshAdditional->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		itemNameWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// Clear the item target
		ptrCharacter->TargetingSystem->ClearTarget();

		// Play pick up aduio
		if (AC_PickUp)
		{
			UGameplayStatics::PlaySoundAtLocation(this, AC_PickUp, GetActorLocation());
		}
	}
}
*/

void AItem::PickUpItem()
{
	if (ptrCharacter->GetInventorySystem()->PickUpItem(this))
	{
		// Dissable object visiblity
		Mesh->SetVisibility(false);
		MeshAdditional->SetVisibility(false);
		itemNameWidget->SetVisibility(false);
		// Dissable collision with mouse
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		MeshAdditional->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		itemNameWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// Clear the item target
		ptrCharacter->GetTargetingSystem()->ClearTarget();

		// Play pick up aduio
		if (AC_PickUp)
		{
			UGameplayStatics::PlaySoundAtLocation(this, AC_PickUp, GetActorLocation());
		}
	}
}

void AItem::SetItemToPickUp(AActor * TouchedActor, FKey ButtonPressed)
{
	ptrCharacter->GetTargetingSystem()->SetTarget(this, ETargetType::ITEM);
}

void AItem::OutlineObject(UPrimitiveComponent * TouchedComponent)
{
	Mesh->SetRenderCustomDepth(true);
	MeshAdditional->SetRenderCustomDepth(true);
}

void AItem::CancelOutlineObject(UPrimitiveComponent * TouchedComponent)
{
	Mesh->SetRenderCustomDepth(false);
	MeshAdditional->SetRenderCustomDepth(false);
}
