// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TreasureDestructible.generated.h"

UCLASS()
class LOD_API ATreasureDestructible : public AActor
{
	GENERATED_BODY()

private:
	/** Root Component */
	UPROPERTY()	USceneComponent * Root;

	/** Treasure mesh */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Others", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent * Mesh;

	/** Particle system which simulate destruction animation */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Others", meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent * PS_DestructibleEffect;

	/** Sound clue, store several breaking sound effects */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Others", meta = (AllowPrivateAccess = "true"))
	class USoundCue * SC_BreakDestructible;

	/** The audio component wraps the USoundCue, and allows us to ineract with it, and its parameters from code */
	UPROPERTY(BlueprintReadOnly, Category = "Others", meta = (AllowPrivateAccess = "true"))
	class UAudioComponent * audioComponent;

	/** Use to call destroy acter after 3 seconds from time he has died */
	FTimerHandle destroyObjectTimer;

public:	
	// Sets default values for this actor's properties
	ATreasureDestructible();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UFUNCTION() void AddDynamics();

	UFUNCTION() void RemoveDynamics();

	UFUNCTION() void DestroyObject();

public:	

	/** Outline  the object */
	UFUNCTION()
	void OutlineObject(UPrimitiveComponent * TouchedComponent);

	/** Do not outline the object */
	UFUNCTION()
	void CancelOutlineObject(UPrimitiveComponent * TouchedComponent);

	/** Check the pick up */
	UFUNCTION()
	void AddObjectIntoTargetingSystem(AActor * ClickedComp, FKey ButtonPressed);

	/** Pick up the item */
	UFUNCTION()
	void BreakObject();
};
