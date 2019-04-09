// Fill out your copyright notice in the Description page of Project Settings.

#include "NPC.h"
#include "../Public/References.h"
#include "../Public/AttributeSystem.h"
#include "../Public/TargetingSystem.h"
#include "../LoDCharacter.h"
#include "../LoDGameMode.h"
#include "../LoDHUD.h"
#include "../Managers/ManagerNPC.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/Texture2D.h"
#include "ConstructorHelpers.h"
#include "../Public/ItemSpawner.h"
#include "../UserInterface/NPC_Name.h"
#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "Engine.h"

#include "../Controllers/AIEnemyController.h"

// Sets default values
ANPC::ANPC()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	/** Create targeting system */
	targetingSystem = CreateDefaultSubobject<UTargetingSystemNPC>(TEXT("TargetingSystem"));

	/** Create attribute system */
	attributeSystem = CreateDefaultSubobject<UAttributeSystem>(TEXT("AttributeSystem"));

	/** Initialize widget component, widget component store item name */
	static ConstructorHelpers::FClassFinder<UUserWidget> hudWidgetObj(TEXT("/Game/HUD/BP_NPC_Widget"));

	// Initialize npc widget 
	this->nameWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Border"));
	if (this->nameWidget)
	{
		this->nameWidget->SetWidgetSpace(EWidgetSpace::World);
		this->nameWidget->SetWidgetClass(hudWidgetObj.Class);
		this->nameWidget->SetupAttachment(RootComponent);
		this->nameWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	GetCharacterMovement()->MaxCustomMovementSpeed = 300.0f;
}


// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();
	
	/** Bind on mouse over events */
	GetMesh()->OnBeginCursorOver.AddDynamic(this, &ANPC::OutlineNPC);
	GetMesh()->OnEndCursorOver.AddDynamic(this, &ANPC::CancelOutlinegNPC);

	/** Bind the mouse event, on mouse click, set the pick up itemSetItemToPickUp to character targeted item */
	OnReleased.AddDynamic(this, &ANPC::OnClickSetToTargetingSystem);
	
	/** Set the name widget position above the NPC */
	this->nameWidget->SetRelativeLocation(FVector(0.0f, 0.0f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 60.0f));
	this->nameWidget->SetPivot(FVector2D(0.5f, 0.02f));
	
	/** Set the widget variables, blueprint widget is inherited from this c++ widget class */
	UNPC_Name * tempWidget = Cast<UNPC_Name>(this->nameWidget->GetUserWidgetObject());
	if (tempWidget) tempWidget->SetUpWidget(this->npcName, this->npcType, this->attributeSystem);


	AAIEnemyController * ptrController = Cast<AAIEnemyController>(GetController());


	/** Determine by what colour will be NPC outlined depends on the NPC type */
	switch (npcType)
	{
	case ENPC_Type::ENEMY:
		GetMesh()->SetCustomDepthStencilValue(1);
		/** Push this NPC into NPC manager */
		if (ptrController)
		{
			ptrGameMode->GetManagerNPC()->PushNewNPC(this);
			ptrController->InitializeControllerVariables(this, EAIBehaviourState::WANDERING);
		}
		break;
	case ENPC_Type::FRIENDLY:
		GetMesh()->SetCustomDepthStencilValue(0);
		if (ptrController)
		{
			ptrController->InitializeControllerVariables(this, EAIBehaviourState::IDLE);
		}
		break;
	case ENPC_Type::SHOP:
		GetMesh()->SetCustomDepthStencilValue(0);
		GetCharacterMovement()->DisableMovement();
		break;
	}
}



/** Outline NPC */
void ANPC::OutlineNPC(UPrimitiveComponent * TouchedComponent)
{
	GetMesh()->SetRenderCustomDepth(true);
}

/** Do not outline NPC */
void ANPC::CancelOutlinegNPC(UPrimitiveComponent * TouchedComponent)
{
	GetMesh()->SetRenderCustomDepth(false);
}

/** Set NPC into targeting system */
void ANPC::OnClickSetToTargetingSystem(AActor * TouchedActor, FKey ButtonPressed)
{
	ptrCharacter->GetTargetingSystem()->SetNPCTarget(this, ETargetType::NPC, this->npcType);
}


UTexture2D * ANPC::GetVisualTexture() const
{
	return this->npcVisualTexture; 
}


void ANPC::PlayAgroSoundEffect()
{
	// Play pick up aduio
	if (this->SE_Agro_01)
	{
		UGameplayStatics::PlaySoundAtLocation(this, this->SE_Agro_01, GetActorLocation());
	}
}

void ANPC::KillNPC()
{
	// Disable collision with NPC
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Remove NPC from the npc manager array, so NPC manager will not longer interact with this NPC
	ptrGameMode->GetManagerNPC()->RemoveNPC(this);

	// Add experience and spawn text
	ptrHUD->SpawnFloatingTextWidget(this, EFloatingTextType::EXPERIENCE_GAINED, 60, false);
	ptrCharacter->GetAttributeSystem()->IncreaseExperience(50);

	// Set controller behaviour to dead, do action stop movement and set animation blueprint to play death animation
	AAIEnemyController * controller = Cast<AAIEnemyController>(GetController());

	// Randomly spawn item
	uint8 numberOfItems = FMath::RandRange(0, 11);
	if (numberOfItems < 5) numberOfItems = 0;
	else if (numberOfItems < 8) numberOfItems = 1;
	else if (numberOfItems < 10) numberOfItems = 2;
	else if (numberOfItems < 20) numberOfItems = 3;

	for (uint8 i = 0; i < numberOfItems; i++)
	{
		ptrGameMode->itemSpawner->SpawnItem((this->GetActorLocation() + FMath::RandRange(0, 70)), FRotator(0.0f, FMath::RandRange(0.0f, 360.0f), 0.0f));
	}


	if (controller)
	{
		controller->behaviourState = EAIBehaviourState::DEAD;
		controller->DoAction();
		GetWorld()->GetTimerManager().SetTimer(this->timer, this, &ANPC::DestroyNPC, 15, false);
	}

	// Disable NPC health and name widget because it will not longer rotate towards to camera, as npc manager maintain the widget rotation to camera
	// Because we have removed this NPC from NPC manager it will no longer manage it
	this->nameWidget->SetVisibility(false);

}