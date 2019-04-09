// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ControlsSettings.h"
#include "VideoSettings.h"
#include "GameSettings.generated.h"

UCLASS()
class LOD_API AGameSettings : public AActor
{
	GENERATED_BODY()
public:
	UPROPERTY()
		USceneComponent * Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ControlSettings")
	class UControlsSettings * ControlsSettings;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ControlSettings")
	class UVideoSettings * VideoSettings;

public:	
	// Sets default values for this actor's properties
	AGameSettings();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
