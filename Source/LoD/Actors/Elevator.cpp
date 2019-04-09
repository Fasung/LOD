// Fill out your copyright notice in the Description page of Project Settings.

#include "Elevator.h"
#include "../LoDCharacter.h"
#include "References.h"
#include "../LoDEnumTypes.h"
#include "../Public/TargetingSystem.h"
#include "Components/StaticMeshComponent.h"

enum ELoDElevatorState
{
	IS_GOING_UP,
	IS_GOING_DOWN,
	IS_WAITING
};

#define ELEVATOR_LEVER_OFF_SET 70

// Sets default values
AElevator::AElevator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	// Initialize elevator moving platform mesh
	MeshElevatorMovingPlatform = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ElevatorMovingPlatform"));
	MeshElevatorMovingPlatform->SetupAttachment(Root);

	// Initialize elevator static platform mesh, this mesh at the bottom of the elevator
	MeshElevatorStaticPlatform = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ElevatorStaticPlatform"));
	MeshElevatorStaticPlatform->SetupAttachment(Root);

	// Initialize second lever mesh
	MeshElevatorLeverStaticPart = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ElevatorLeverStaticPart"));
	MeshElevatorLeverStaticPart->SetupAttachment(Root);
	MeshElevatorLeverDynamicPart = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ElevatorLeverDynamicPart"));
	MeshElevatorLeverDynamicPart->SetupAttachment(Root);

	// Initialize first lever mesh
	MeshLeverTopStaticPart = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TopLeverStaticPart"));
	MeshLeverTopStaticPart->SetupAttachment(Root);
	MeshLeverTopDynamicPart = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TopLeverDynamicPart"));
	MeshLeverTopDynamicPart->SetupAttachment(Root);

	// Initialize second lever mesh
	MeshLeverBottomStaticPart = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BottomLeverStaticPart"));
	MeshLeverBottomStaticPart->SetupAttachment(Root);
	MeshLeverBottomDynamicPart = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BottomLeverDynamicPart"));
	MeshLeverBottomDynamicPart->SetupAttachment(Root);

	// Dynamic part is does not need to affect navigation because it is above the static part of lever which affect navigation
	MeshLeverTopDynamicPart->SetCanEverAffectNavigation(false);
	MeshLeverBottomDynamicPart->SetCanEverAffectNavigation(false);
}

// Called when the game starts or when spawned
void AElevator::BeginPlay()
{
	Super::BeginPlay();
	elevatorCurrentState = ELoDElevatorState::IS_WAITING;
	elevatorPreviousState = ELoDElevatorState::IS_GOING_UP;

	/** Bind the mouse events on mouse click, interact with elevator states */
	AddDynamics();

	// Initialize vector elevatorLocation, because elevator will move only in set coordinates, there is no nead the do function call GetActorLocation()
	elevatorLocation = MeshElevatorMovingPlatform->GetComponentLocation();
	elevatorLeverLocation = elevatorLocation;
	elevatorLeverLocation.Z += ELEVATOR_LEVER_OFF_SET;

	MeshElevatorLeverStaticPart->SetWorldLocation(elevatorLeverLocation);
	MeshElevatorLeverDynamicPart->SetWorldLocation(elevatorLeverLocation);
}

void AElevator::AddDynamics()
{
	// Add mouse click events
	MeshLeverTopStaticPart->OnReleased.AddDynamic(this, &AElevator::TurnTopLever);
	MeshLeverTopDynamicPart->OnReleased.AddDynamic(this, &AElevator::TurnTopLever);
	MeshLeverBottomStaticPart->OnReleased.AddDynamic(this, &AElevator::TurnBottomLever);
	MeshLeverBottomDynamicPart->OnReleased.AddDynamic(this, &AElevator::TurnBottomLever);
	MeshElevatorLeverStaticPart->OnReleased.AddDynamic(this, &AElevator::TurnElevatorLever);
	MeshElevatorLeverDynamicPart->OnReleased.AddDynamic(this, &AElevator::TurnElevatorLever);

	// Add mouse over events
	MeshLeverTopStaticPart->OnBeginCursorOver.AddDynamic(this, &AElevator::OutlineOtherLevers);
	MeshLeverTopStaticPart->OnEndCursorOver.AddDynamic(this, &AElevator::CancelOutliningOtherLevers);
	MeshLeverTopDynamicPart->OnBeginCursorOver.AddDynamic(this, &AElevator::OutlineOtherLevers);
	MeshLeverTopDynamicPart->OnEndCursorOver.AddDynamic(this, &AElevator::CancelOutliningOtherLevers);

	MeshLeverBottomStaticPart->OnBeginCursorOver.AddDynamic(this, &AElevator::OutlineOtherLevers);
	MeshLeverBottomStaticPart->OnEndCursorOver.AddDynamic(this, &AElevator::CancelOutliningOtherLevers);
	MeshLeverBottomDynamicPart->OnBeginCursorOver.AddDynamic(this, &AElevator::OutlineOtherLevers);
	MeshLeverBottomDynamicPart->OnEndCursorOver.AddDynamic(this, &AElevator::CancelOutliningOtherLevers);

	MeshElevatorLeverStaticPart->OnBeginCursorOver.AddDynamic(this, &AElevator::OutlineElevatorLever);
	MeshElevatorLeverStaticPart->OnEndCursorOver.AddDynamic(this, &AElevator::CancelOutliningElevatorLever);
	MeshElevatorLeverDynamicPart->OnBeginCursorOver.AddDynamic(this, &AElevator::OutlineElevatorLever);
	MeshElevatorLeverDynamicPart->OnEndCursorOver.AddDynamic(this, &AElevator::CancelOutliningElevatorLever);
}

void AElevator::RemoveDynamics()
{
	// Remove mouse click events
	MeshLeverTopStaticPart->OnReleased.RemoveDynamic(this, &AElevator::TurnTopLever);
	MeshLeverTopDynamicPart->OnReleased.RemoveDynamic(this, &AElevator::TurnTopLever);
	MeshLeverBottomStaticPart->OnReleased.RemoveDynamic(this, &AElevator::TurnBottomLever);
	MeshLeverBottomDynamicPart->OnReleased.RemoveDynamic(this, &AElevator::TurnBottomLever);
	MeshElevatorLeverStaticPart->OnReleased.RemoveDynamic(this, &AElevator::TurnElevatorLever);
	MeshElevatorLeverDynamicPart->OnReleased.RemoveDynamic(this, &AElevator::TurnElevatorLever);

	// Remove mouse over events
	MeshLeverTopStaticPart->OnBeginCursorOver.RemoveDynamic(this, &AElevator::OutlineOtherLevers);
	MeshLeverTopStaticPart->OnEndCursorOver.RemoveDynamic(this, &AElevator::CancelOutliningOtherLevers);
	MeshLeverTopDynamicPart->OnBeginCursorOver.RemoveDynamic(this, &AElevator::OutlineOtherLevers);
	MeshLeverTopDynamicPart->OnEndCursorOver.RemoveDynamic(this, &AElevator::CancelOutliningOtherLevers);

	MeshLeverBottomStaticPart->OnBeginCursorOver.RemoveDynamic(this, &AElevator::OutlineOtherLevers);
	MeshLeverBottomStaticPart->OnEndCursorOver.RemoveDynamic(this, &AElevator::CancelOutliningOtherLevers);
	MeshLeverBottomDynamicPart->OnBeginCursorOver.RemoveDynamic(this, &AElevator::OutlineOtherLevers);
	MeshLeverBottomDynamicPart->OnEndCursorOver.RemoveDynamic(this, &AElevator::CancelOutliningOtherLevers);

	MeshElevatorLeverStaticPart->OnBeginCursorOver.RemoveDynamic(this, &AElevator::OutlineElevatorLever);
	MeshElevatorLeverStaticPart->OnEndCursorOver.RemoveDynamic(this, &AElevator::CancelOutliningElevatorLever);
	MeshElevatorLeverDynamicPart->OnBeginCursorOver.RemoveDynamic(this, &AElevator::OutlineElevatorLever);
	MeshElevatorLeverDynamicPart->OnEndCursorOver.RemoveDynamic(this, &AElevator::CancelOutliningElevatorLever);
}

// Called every frame
void AElevator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (elevatorCurrentState)
	{
	case ELoDElevatorState::IS_GOING_UP:
		// Calculate new elevator position
		elevatorLocation.Z += ELEVATOR_VELOCITY * DeltaTime;
		// Calculate new position of lever based in elevator
		elevatorLeverLocation = elevatorLocation;
		elevatorLeverLocation.Z += ELEVATOR_LEVER_OFF_SET;
		// Assign new positions into meshes
		MeshElevatorMovingPlatform->SetWorldLocation(elevatorLocation);
		MeshElevatorLeverStaticPart->SetWorldLocation(elevatorLeverLocation);
		MeshElevatorLeverDynamicPart->SetWorldLocation(elevatorLeverLocation);

		// Elevator reached top point
		if (elevatorLocation.Z > elevatorTopPoint)
		{
			// Make sure that elevator position is not higher than is allowed position
			elevatorLocation.Z = elevatorTopPoint;
			// Calculate final position of lever based in elevator
			elevatorLeverLocation = elevatorLocation;
			elevatorLeverLocation.Z += ELEVATOR_LEVER_OFF_SET;
			// Set final positions of elevator meshes
			MeshElevatorMovingPlatform->SetWorldLocation(elevatorLocation);
			MeshElevatorLeverStaticPart->SetWorldLocation(elevatorLeverLocation);
			MeshElevatorLeverDynamicPart->SetWorldLocation(elevatorLeverLocation);
			// Change elevator state and disable tick function
			elevatorCurrentState = IS_WAITING;
			this->SetActorTickEnabled(false);
		}
		break;
	case ELoDElevatorState::IS_GOING_DOWN:
		// Calculate new elevator position
		elevatorLocation.Z -= ELEVATOR_VELOCITY * DeltaTime;
		// Calculate new position of lever based in elevator
		elevatorLeverLocation = elevatorLocation;
		elevatorLeverLocation.Z += ELEVATOR_LEVER_OFF_SET;
		// Assign new positions into meshes
		MeshElevatorMovingPlatform->SetWorldLocation(elevatorLocation);
		MeshElevatorLeverStaticPart->SetWorldLocation(elevatorLeverLocation);
		MeshElevatorLeverDynamicPart->SetWorldLocation(elevatorLeverLocation);

		// Elevator reached bottom point
		if (elevatorLocation.Z < elevatorBottomPoint)
		{
			// Make sure that elevator position is not lower than is allowed position
			elevatorLocation.Z = elevatorBottomPoint;
			// Calculate final position of lever based in elevator
			elevatorLeverLocation = elevatorLocation;
			elevatorLeverLocation.Z += ELEVATOR_LEVER_OFF_SET;
			// Set final positions of elevator meshes
			MeshElevatorMovingPlatform->SetWorldLocation(elevatorLocation);
			MeshElevatorLeverStaticPart->SetWorldLocation(elevatorLeverLocation);
			MeshElevatorLeverDynamicPart->SetWorldLocation(elevatorLeverLocation);
			// Change elevator state and disable tick function
			elevatorCurrentState = IS_WAITING;
			this->SetActorTickEnabled(false);
		}
		break;
	case ELoDElevatorState::IS_WAITING:
		// PRINT DEBUG, ELEVATOR SHOULD MOVE AND NOT BE IN WAITING STATE
		this->SetActorTickEnabled(false);
		break;
	}
}

void AElevator::MoveElevator()
{
	if (elevatorCurrentState == ELoDElevatorState::IS_WAITING)
	{
		if (elevatorPreviousState == ELoDElevatorState::IS_GOING_DOWN)
		{
			elevatorPreviousState = ELoDElevatorState::IS_GOING_UP;
			elevatorCurrentState = ELoDElevatorState::IS_GOING_UP;
		}
		else
		{
			elevatorPreviousState = ELoDElevatorState::IS_GOING_DOWN;
			elevatorCurrentState = ELoDElevatorState::IS_GOING_DOWN;
		}

		this->SetActorTickEnabled(true);
		ptrCharacter->GetTargetingSystem()->ClearTarget();
	}
}


////////////////////////////
///// Events Functions /////
////////////////////////////

void AElevator::TurnElevatorLever(UPrimitiveComponent* TouchedComponent, FKey key)
{
	ptrCharacter->GetTargetingSystem()->SetStaticTarget(this, ETargetType::ELEVATOR, MeshElevatorLeverStaticPart->GetComponentLocation());
}

void AElevator::TurnTopLever(UPrimitiveComponent* TouchedComponent, FKey key)
{
	ptrCharacter->GetTargetingSystem()->SetStaticTarget(this, ETargetType::ELEVATOR, MeshLeverTopStaticPart->GetComponentLocation());
}

void AElevator::TurnBottomLever(UPrimitiveComponent* TouchedComponent, FKey key)
{
	ptrCharacter->GetTargetingSystem()->SetStaticTarget(this, ETargetType::ELEVATOR, MeshLeverBottomStaticPart->GetComponentLocation());
}

void AElevator::OutlineElevatorLever(UPrimitiveComponent * TouchedComponent)
{
	MeshElevatorLeverStaticPart->SetRenderCustomDepth(true);
	MeshElevatorLeverDynamicPart->SetRenderCustomDepth(true);
}

void AElevator::CancelOutliningElevatorLever(UPrimitiveComponent * TouchedComponent)
{
	MeshElevatorLeverStaticPart->SetRenderCustomDepth(false);
	MeshElevatorLeverDynamicPart->SetRenderCustomDepth(false);
}

void AElevator::OutlineOtherLevers(UPrimitiveComponent * TouchedComponent)
{
	MeshLeverTopStaticPart->SetRenderCustomDepth(true);
	MeshLeverTopDynamicPart->SetRenderCustomDepth(true);
	MeshLeverBottomStaticPart->SetRenderCustomDepth(true);
	MeshLeverBottomDynamicPart->SetRenderCustomDepth(true);
}

void AElevator::CancelOutliningOtherLevers(UPrimitiveComponent * TouchedComponent)
{
	MeshLeverTopStaticPart->SetRenderCustomDepth(false);
	MeshLeverTopDynamicPart->SetRenderCustomDepth(false);
	MeshLeverBottomStaticPart->SetRenderCustomDepth(false);
	MeshLeverBottomDynamicPart->SetRenderCustomDepth(false);
}