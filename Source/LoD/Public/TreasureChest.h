// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TreasureChest.generated.h"

UCLASS()
class LOD_API ATreasureChest : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY()
	USceneComponent * Root;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent * Mesh_TopPart;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent * Mesh_BottomPart;

	/** Sound to play when player open chest */
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundBase * AC_OpenChest;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	/** Outline  the object */
	UFUNCTION()
	void OutlineObject(UPrimitiveComponent * TouchedComponent);

	/** Do not outline the object */
	UFUNCTION()
	void CancelOutlineObject(UPrimitiveComponent * TouchedComponent);

	/** Check the pick up */
	UFUNCTION()
	void SetToTarget(AActor * ClickedComp, FKey ButtonPressed);

	/** Pick up the item*/
	UFUNCTION(BlueprintImplementableEvent, Category = "OpenChest")
	void OpenChest();
	
	/** Pick up the item*/
	UFUNCTION(BlueprintCallable, Category = "RemoveDynamics")
	void RemoveDynamics();

public:
	// Sets default values for this actor's properties
	ATreasureChest();
	
};
