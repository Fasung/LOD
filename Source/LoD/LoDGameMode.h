// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LoDGameMode.generated.h"

UCLASS(minimalapi)
class ALoDGameMode : public AGameModeBase
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

public:
	ALoDGameMode();
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hud", Meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> PlayerHudClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainWidget", meta = (AllowPrivateAccess = "true"))
	class UUserWidget * CurrentWidget;


public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemSpawner")
	class UItemSpawner * itemSpawner;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ManagerNPC")
	class UManagerNPC * managerNPC;

private:

	UFUNCTION(BlueprintCallable, Category = "MainWidget", meta = (AllowPrivateAccess = "true"))
	class UUserWidget * GetGameplayHUD()	const { return this->CurrentWidget;  }

public: /** Functions */

	FORCEINLINE class UManagerNPC * GetManagerNPC() const { return this->managerNPC; }

	FORCEINLINE TSubclassOf<class UUserWidget> GetMainHud() const { return this->PlayerHudClass; }

};
