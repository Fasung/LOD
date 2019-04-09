// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LoDEnumTypes.h"
#include "LoDHUD.generated.h"


/**
 * 
 */
UCLASS()
class LOD_API ALoDHUD : public AHUD
{
	GENERATED_BODY()


protected: /** Functions */
//	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hud", Meta = (BlueprintProtected = "true"))
//	TSubclassOf<class UUserWidget> playerHudClass;

//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainWidget", meta = (AllowPrivateAccess = "true"))
//	class UUserWidget * currentWidget;

public:

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Widget"))
	void SpawnFloatingTextWidget(const class APawn * characterBellowText, const EFloatingTextType textType, const int value, const bool IsCriticalDamage = false);

public:
	ALoDHUD();
};
