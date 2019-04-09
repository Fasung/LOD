// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "LoDPlayerController.h"
#include "LoDCharacter.h"
#include "LoDHUD.h"
#include "Components/DecalComponent.h"
#include "Components/PostProcessComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "AnimationInstances/HumanSkeletonAnimInstance.h"
#include "Components/ShopSystem.h"
#include "Public/EquipmentSystem.h"
#include "Public/InventorySystem.h"
#include "Public/TargetingSystem.h"
#include "AI/Navigation/NavigationSystem.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "References.h"
#include "Actors/NPC.h"
#include "Settings/ControlsSettings.h"
#include "Settings/GameSettings.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Engine.h"


ALoDPlayerController::ALoDPlayerController() :
					bRightButtonPressed(false),
					cameraCurrentZoom(CAMERA_DEFAULT_ZOOM),
					cameraZoomIntensity(CAMERA_DEFAULT_ZOOM_SENSITIVITY),
					cameraRotationIntensity(CAMERA_DEFAULT_ROTATION_SENSITIVITY)
{
	bShowMouseCursor = true;
	bEnableMouseOverEvents = true;
	bEnableClickEvents = true;
	DefaultMouseCursor = EMouseCursor::Default;
	ptrPlayerController = this;

	shopSystem = CreateDefaultSubobject<UShopSystem>(TEXT("ShopSystem"));
}

void ALoDPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor && ptrCharacter->GetTargetingSystem()->HasTarget() == false)
	{
		MoveToMouseCursor();
	}
	else if (ptrCharacter->GetTargetingSystem()->HasTargetNPC())
	{
		float const Distance = FVector::Dist(ptrCharacter->GetTargetingSystem()->GetTarget()->GetActorLocation(), ptrCharacter->GetActorLocation());
		UNavigationSystem * const NavSys = GetWorld()->GetNavigationSystem();

		if (NavSys && (Distance > 120.0f))
		{
			NavSys->SimpleMoveToActor(this, ptrCharacter->GetTargetingSystem()->GetTarget());
		}
		else
		{
			if (ptrCharacter->GetCharacterMovement()->MovementMode)
			{
				if (ptrCharacter->GetTargetingSystem()->GetTargetNPC()->GetType() == ENPC_Type::ENEMY) ptrCharacter->GetAnimationSystem()->IsMeleeAttacking = true;
				ptrCharacter->GetCharacterMovement()->DisableMovement();
			}
			// Rotate character to the target
			FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(ptrCharacter->GetActorLocation(), ptrCharacter->GetTargetingSystem()->GetTarget()->GetActorLocation());
			newRotation.Roll = 0;
			newRotation.Pitch = 0;
			ptrCharacter->SetActorRotation(newRotation);
		}
	}
}

void ALoDPlayerController::BeginPlay()
{
	Super::BeginPlay();

	/** Initialize camera boom with camera controlls variables */
	ptrCharacter->GetCameraBoom()->TargetArmLength = this->cameraCurrentZoom;
	ptrCharacter->GetCameraBoom()->RelativeRotation = FRotator(-CAMERA_DEFAULT_PITCH, 0.f, 0.f);
}

void ALoDPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Character movement actions, controlled by mouse click
	InputComponent->BindAction("SetDestination", IE_Pressed, this, &ALoDPlayerController::OnLeftMouseButtonDown);
	InputComponent->BindAction("SetDestination", IE_Released, this, &ALoDPlayerController::OnLeftMouseButtonUp);

	// Camera controllers actions, controled by mouse clicks and scrolls
	InputComponent->BindAxis("ZoomIn", this, &ALoDPlayerController::OnCameraZoom);
	InputComponent->BindAxis("RotateCamera", this, &ALoDPlayerController::OnCameraRotate);
	InputComponent->BindAction("IsRightMouseButtonDownOrNot", IE_Pressed, this, &ALoDPlayerController::OnRightMouseButtonDown);
	InputComponent->BindAction("IsRightMouseButtonDownOrNot", IE_Released, this, &ALoDPlayerController::OnRightMouseButtonUp);

	// Character use abilities actions, controlled by keyboard key down
	InputComponent->BindKey(ptrSettings->ControlsSettings->AbilityOne, IE_Pressed, this, &ALoDPlayerController::OnAbilityOnePressed);
	InputComponent->BindKey(ptrSettings->ControlsSettings->AbilityTwo, IE_Pressed, this, &ALoDPlayerController::OnAbilityTwoPressed);
	InputComponent->BindKey(ptrSettings->ControlsSettings->AbilityThree, IE_Pressed, this, &ALoDPlayerController::OnAbilityThreePressed);
	InputComponent->BindKey(ptrSettings->ControlsSettings->AbilityFour, IE_Pressed, this, &ALoDPlayerController::OnAbilityFourPressed);
	InputComponent->BindKey(ptrSettings->ControlsSettings->AbilityFive, IE_Pressed, this, &ALoDPlayerController::OnAbilityFivePressed);

	// Drinking potions actions, controlled by keyboard key down
	InputComponent->BindKey(ptrSettings->ControlsSettings->DrinkHealthPotion, IE_Pressed, this, &ALoDPlayerController::OnDrinkHealthPotionPressed);
	InputComponent->BindKey(ptrSettings->ControlsSettings->DrinkManaPotion, IE_Pressed, this, &ALoDPlayerController::OnDrinkManaPotionPressed);

	// User Interface keyboard key down actions
	InputComponent->BindKey(ptrSettings->ControlsSettings->OpenInventory, IE_Released, this, &ALoDPlayerController::OnOpenInventoryPressed);
	InputComponent->BindKey(ptrSettings->ControlsSettings->OpenPauseMenu, IE_Released, this, &ALoDPlayerController::OnOpenPauseMenuPressed);
	InputComponent->BindKey(ptrSettings->ControlsSettings->OpenQuestLog, IE_Released, this, &ALoDPlayerController::OnOpenQuestLogPressed);
	InputComponent->BindKey(ptrSettings->ControlsSettings->OpenSpellBook, IE_Released, this, &ALoDPlayerController::OnOpenSpellBookPressed);
	
}

void ALoDPlayerController::MoveToMouseCursor()
{
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		if (ALoDCharacter* MyPawn = Cast<ALoDCharacter>(GetPawn()))
		{
			if (MyPawn->GetCursorToWorld())
			{
				UNavigationSystem::SimpleMoveToLocation(this, MyPawn->GetCursorToWorld()->GetComponentLocation());
			}
		}
	}
	else
	{
		// Trace to see what is under the mouse cursor
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit)
		{
			// We hit something, move there
			SetNewMoveDestination(Hit.ImpactPoint);
		}
	}
}

void ALoDPlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		UNavigationSystem* const NavSys = GetWorld()->GetNavigationSystem();
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if (NavSys && (Distance > 120.0f))
		{
			NavSys->SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void ALoDPlayerController::OnLeftMouseButtonDown()
{
	// set flag to keep updating destination until released
	bMoveToMouseCursor = true;
	this->bRightButtonPressed = false;

	/** If the movement is disabled, enable it */
	if (!ptrCharacter->GetCharacterMovement()->MovementMode) ptrCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	// Clear targeting system when player left click
	if (ptrCharacter->GetTargetingSystem()->HasTarget()) ptrCharacter->GetTargetingSystem()->ClearTarget();
}

void ALoDPlayerController::OnLeftMouseButtonUp()
{
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
}


void ALoDPlayerController::OnCameraZoom(float scrollValue)
{
	this->cameraCurrentZoom -= scrollValue * this->cameraZoomIntensity;

	if (this->cameraCurrentZoom < CAMERA_MINIMUM_ZOOM) this->cameraCurrentZoom = CAMERA_MINIMUM_ZOOM;
	else if (this->cameraCurrentZoom > CAMERA_MAXIMUM_ZOOM) this->cameraCurrentZoom = CAMERA_MAXIMUM_ZOOM;

	ptrCharacter->GetCameraBoom()->TargetArmLength = this->cameraCurrentZoom;
}

void ALoDPlayerController::OnCameraRotate(float moveValue)
{
	if (this->bRightButtonPressed)
	{
		/*
		if (ptrGameMode)
		{
			if (ptrGameMode->VideoSettings)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("VIDEO SETTINGS IS POINTING TO SOMETHING !!"));


			if (GetWorld())
			{
				// IMPORTANT TODO:
				// IMPORTANT TODO:
				// IMPORTANT TODO:
				// Once we will have implemented functionality between the begin play and tick function, will move this functions over there
				// at the moment in begin play, assigned values are 0, and it is non sense to be updating something one can happen only once 
				// per application execution every frame
				//ptrGameMode->GetVideoSettings()->UpdateViewportCenter();
			}
			//ptrGameMode->GetVideoSettings()->UpdateViewportCenter();
			if (ptrGameMode->VideoSettings) SetMouseLocation(ptrGameMode->VideoSettings->viewportCenter.X, ptrGameMode->VideoSettings->viewportCenter.Y);
			
			*/
			ptrCharacter->GetCameraBoom()->SetRelativeRotation(FRotator(-CAMERA_DEFAULT_PITCH, ptrCharacter->GetCameraBoom()->RelativeRotation.Yaw + moveValue * cameraRotationIntensity, 0.f));
		//}
	}
}

void ALoDPlayerController::OnRightMouseButtonDown()
{
	this->bRightButtonPressed = true;
	//GetMousePosition(lastMousePosition.X, lastMousePosition.Y);
}

void ALoDPlayerController::OnRightMouseButtonUp()
{
	this->bRightButtonPressed = false;
	//SetMouseLocation(lastMousePosition.X, lastMousePosition.Y);
}


void ALoDPlayerController::OnDrinkHealthPotionPressed()
{
	
	if (ptrCharacter->GetInventorySystem()->HasHealthPotion())
	{
		ptrHUD->SpawnFloatingTextWidget(ptrCharacter, EFloatingTextType::HEALTH_GAINED, 120, false);
		ptrCharacter->GetAnimationSystem()->StartDrinkingHealthPotion();
	}
	
}

void ALoDPlayerController::OnDrinkManaPotionPressed()
{
	
	if (ptrCharacter->GetInventorySystem()->HasManaPotion())
	{
		ptrHUD->SpawnFloatingTextWidget(ptrCharacter, EFloatingTextType::MANA_GAINED, 120, false);
		ptrCharacter->GetAnimationSystem()->StartDrinkingManaPotion();
	}
	
}

void ALoDPlayerController::OnAbilityOnePressed()			{ ptrCharacter->GetAnimationSystem()->IsMeleeAttacking = true; }
void ALoDPlayerController::OnAbilityTwoPressed()			{}
void ALoDPlayerController::OnAbilityThreePressed()			{}
void ALoDPlayerController::OnAbilityFourPressed()			{}
void ALoDPlayerController::OnAbilityFivePressed()			{}

void ALoDPlayerController::OnOpenInventoryPressed()			
{
	if (ptrCharacter->GetInventorySystem()->IsOpened())	ptrCharacter->GetInventorySystem()->CloseInventory();
	else ptrCharacter->GetInventorySystem()->OpenInventory();
}

void ALoDPlayerController::OnOpenPauseMenuPressed()			{}
void ALoDPlayerController::OnOpenQuestLogPressed()			{}
void ALoDPlayerController::OnOpenSpellBookPressed()			{}
