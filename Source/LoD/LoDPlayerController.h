// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LoDPlayerController.generated.h"

// IMPORTANT TODO: gameMode->GetVideoSettings()->UpdateViewportCenter(); move this function away!!
// Once we will have implemented functionality between the begin play and tick function, will move this functions over there
// at the moment in begin play, assigned values are 0, and it is non sense to be updating something one can happen only once 
// per application execution every frame
// "gameMode->GetVideoSettings()->UpdateViewportCenter();" move this function away!!


UCLASS()
class ALoDPlayerController : public APlayerController
{
	GENERATED_BODY()
		
private:
	/** Camera properties*/
	/** Constant camera properties */
	const float CAMERA_MINIMUM_ZOOM = 300.0f;
	const float CAMERA_DEFAULT_ZOOM = 800.0f;
	const float CAMERA_MAXIMUM_ZOOM = 1200.0f;
	const float CAMERA_DEFAULT_PITCH = 41.0f;
	const float CAMERA_DEFAULT_ZOOM_SENSITIVITY = 100.0f;
	const float CAMERA_DEFAULT_ROTATION_SENSITIVITY = 1.5f;

	/** Amenable camera properties*/
	float cameraCurrentZoom;
	float cameraZoomIntensity;
	float cameraRotationIntensity;
	const FVector2D cameraRotationMoveValue;

	/** Mouse properties */
	uint32 bRightButtonPressed : 1;

	/** ViewPort variables store center of the screen, used for smoother camera rotation */
	FVector2D lastMousePosition;

public:
	UPROPERTY(BlueprintReadOnly, Category = "ShopSystem")
	class UShopSystem * shopSystem;

private:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
	/** Navigate player to the current mouse cursor location. */
	void MoveToMouseCursor();

	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation);

	/** Input handlers for main action bar, manage character abilities and potion drink buttons
	* Ability actions*/	
	void OnAbilityOnePressed();
	void OnAbilityTwoPressed();
	void OnAbilityThreePressed();
	void OnAbilityFourPressed();
	void OnAbilityFivePressed();
	
	/** Potion drinking actions */
	void OnDrinkManaPotionPressed();
	void OnDrinkHealthPotionPressed();
	
	/** Input handlers for secondary action bar, manage user interface buttons, inventory, pause menu, spell book, quest log
	* User Interface actions*/
	void OnOpenInventoryPressed();
	void OnOpenPauseMenuPressed();
	void OnOpenQuestLogPressed();
	void OnOpenSpellBookPressed();
	




	
	void OnCameraZoom(float scrollValue);
	void OnCameraRotate(float moveValue);
	
	/** Input handlers for right mouse button action. */
	void OnRightMouseButtonDown();
	void OnRightMouseButtonUp();

	/** Input handlers for left mouse button action. */
	void OnLeftMouseButtonDown();
	void OnLeftMouseButtonUp();

public:



	const FORCEINLINE bool GetRightMouseButtonState()				{ return bRightButtonPressed; }
	
public: /** Constructors */
	ALoDPlayerController();
};



