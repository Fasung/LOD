// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/PointLightComponent.h"
#include "Torch.generated.h"


UCLASS()
class LOD_API ATorch : public AActor
{
	GENERATED_BODY()
public:
	UPROPERTY()
	USceneComponent * Root;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent * MeshTorch;

	UPROPERTY(VisibleAnywhere)
	UPointLightComponent * LightEffect;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	// Sets default values for this actor's properties
	ATorch();
	
	
};
