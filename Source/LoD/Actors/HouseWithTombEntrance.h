// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HouseWithTombEntrance.generated.h"

UCLASS()
class LOD_API AHouseWithTombEntrance : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere)
	USceneComponent * Root;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent * MeshRoof;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent * MeshHouse;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent * MeshLeftTombDoor;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent * MeshRightTombDoor;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// Sets default values for this actor's properties
	AHouseWithTombEntrance();
	
};
