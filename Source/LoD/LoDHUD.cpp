// Fill out your copyright notice in the Description page of Project Settings.

#include "LoDHUD.h"
#include "References.h"
#include "LoDCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "Camera/CameraComponent.h"

ALoDHUD::ALoDHUD()
{
//	if (playerHudClass != nullptr)
//	{
	//	currentWidget = CreateWidget<UUserWidget>(GetWorld(), playerHudClass);
//	}

//	dynamic_cast<UUserWidget*>(playerHudClass)->AddToViewport();
	//currentWidget->AddToViewport();

	ptrHUD = this;
}

