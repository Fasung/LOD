// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Elevator.generated.h"

UCLASS()
class LOD_API AElevator : public AActor
{
	GENERATED_BODY()
	
public:	

	UPROPERTY()
	USceneComponent * Root;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent * MeshElevatorMovingPlatform;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent * MeshElevatorStaticPlatform;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent * MeshLeverTopStaticPart;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent * MeshLeverTopDynamicPart;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent * MeshLeverBottomStaticPart;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent * MeshLeverBottomDynamicPart;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent * MeshElevatorLeverStaticPart; 

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent * MeshElevatorLeverDynamicPart;

private:
	uint8 elevatorCurrentState;
	uint8 elevatorPreviousState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ElevatorPositions", meta = (AllowPrivateAccess = "true"))
	float elevatorTopPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ElevatorPositions", meta = (AllowPrivateAccess = "true"))
	float elevatorBottomPoint;

	const float ELEVATOR_VELOCITY = 150.0f;
	FVector elevatorLocation;
	FVector elevatorLeverLocation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Set bottom lever into the targeting system, when player is close enough to lever, it will execute elevator movement */
	UFUNCTION()
	void TurnBottomLever(UPrimitiveComponent * TouchedComponent, FKey key);

	/** Set top into the targeting system, when player is close enough to lever, it will execute elevator movement */
	UFUNCTION()
	void TurnTopLever(UPrimitiveComponent * TouchedComponent, FKey key);

	/** Set top into the targeting system, when player is close enough to lever, it will execute elevator movement */
	UFUNCTION()
	void TurnElevatorLever(UPrimitiveComponent * TouchedComponent, FKey key);

	UFUNCTION()
	void OutlineElevatorLever(UPrimitiveComponent * TouchedComponent);

	UFUNCTION()
	void CancelOutliningElevatorLever(UPrimitiveComponent * TouchedComponent);

	UFUNCTION()
	void OutlineOtherLevers(UPrimitiveComponent * TouchedComponent);

	UFUNCTION()
	void CancelOutliningOtherLevers(UPrimitiveComponent * TouchedComponent);

	/** Called from targeting system, player is close enough to lever and can push it */
	UFUNCTION()
	void MoveElevator();

	UFUNCTION()
	void AddDynamics();

	UFUNCTION()
	void RemoveDynamics();
public:
	// Sets default values for this actor's properties
	AElevator();
	
};
