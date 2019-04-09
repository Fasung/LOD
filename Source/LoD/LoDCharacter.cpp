// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "LoDCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "AttributeSystem.h"
#include "EquipmentSystem.h"
#include "InventorySystem.h"
#include "TargetingSystem.h"
#include "../Components/ShopSystem.h"
#include "../LoDPlayerController.h"
#include "../AnimationInstances/HumanSkeletonAnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "References.h"
#include "Engine.h"

ALoDCharacter::ALoDCharacter()
{
	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	
	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	GetCharacterMovement()->MaxCustomMovementSpeed = 500.0f;

	// Create a camera boom...
	cameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	cameraBoom->SetupAttachment(RootComponent);
	cameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	// Rest of cameraBoom is initialized in LoDPlayerController

	// Create a camera...
	cameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	cameraComponent->SetupAttachment(cameraBoom, USpringArmComponent::SocketName);
	cameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create attribute system
	attributeSystem = CreateDefaultSubobject<UAttributeSystem>(TEXT("AttributeSystem"));

	// Create equipment system
	equipmentSystem = CreateDefaultSubobject<UEquipmentSystem>(TEXT("EquipmentSystem"));
	equipmentSystem->AttachSkeletalMesh(GetMesh());
	equipmentSystem->AttachAttributeSystem(attributeSystem);

	// Create inventory system
	inventorySystem = CreateDefaultSubobject<UInventorySystem>(TEXT("InventorySystem"));

	// Create targeting system
	targetingSystem = CreateDefaultSubobject<UTargetingSystem>(TEXT("TargetingSystem"));

	// Create a decal in the world to show the cursor's location
	cursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	cursorToWorld->SetupAttachment(RootComponent);
	
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
	
	if (DecalMaterialAsset.Succeeded())
	{
		cursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	
	cursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	cursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());
	cursorToWorld->SetVisibility(true);

	// Initialize level up particle system
	PSSSS_levelUp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PS_LevelUp"));
	this->attributeSystem->AttachLevelUpParticleSystem(PSSSS_levelUp);

	// Set global character pointer into this character	
	ptrCharacter = this;
}

void ALoDCharacter::BeginPlay()
{
	Super::BeginPlay();

	/** Initialize animation system, it has to be initialize after the LoDCharacter and animation instance blueprint has been created,
		otherwise pointer would be set into null pointer, which would cause fatal errors */
	animationSystem = dynamic_cast<UHumanSkeletonAnimInstance*>(GetMesh()->GetAnimInstance());

	PSSSS_levelUp->AttachTo(RootComponent);
	PSSSS_levelUp->SetRelativeLocation(FVector(0.0f, 0.0f, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
	PSSSS_levelUp->Deactivate();
}

void ALoDCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// If character has target, update targeting system 
	if (targetingSystem->HasTarget()) targetingSystem->UpdateTargetingSystem();

	// If shop is opened, check player distance from the shop, if player is too far, close shop 
	if (ptrPlayerController->shopSystem->IsOpened())
	{
		// Calculate displacement
		ptrPlayerController->shopSystem->shopDistanceToPlayer = ptrPlayerController->shopSystem->shopPosition - this->GetActorLocation();
		// Calculate not squared distance
		ptrPlayerController->shopSystem->shopDistanceToPlayer.Z = ptrPlayerController->shopSystem->shopDistanceToPlayer.X * ptrPlayerController->shopSystem->shopDistanceToPlayer.X +
																		ptrPlayerController->shopSystem->shopDistanceToPlayer.Y * ptrPlayerController->shopSystem->shopDistanceToPlayer.Y;
		// If distance is negative, multiply it by -1 to get positive number
		if (ptrPlayerController->shopSystem->shopDistanceToPlayer.Z < 0.0f) ptrPlayerController->shopSystem->shopDistanceToPlayer.Z *= -1.0f;

		if (ptrPlayerController->shopSystem->shopDistanceToPlayer.Z > 65535.0f)
		{
			ptrPlayerController->shopSystem->CloseShop();
			this->inventorySystem->CloseInventory();
		}
	}

	if (cursorToWorld != nullptr)
	{
		if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
		{
			if (UWorld* World = GetWorld())
			{
				FHitResult HitResult;
				FCollisionQueryParams Params(NAME_None, FCollisionQueryParams::GetUnknownStatId());
				FVector StartLocation = cameraComponent->GetComponentLocation();
				FVector EndLocation = cameraComponent->GetComponentRotation().Vector() * 2000.0f;
				Params.AddIgnoredActor(this);
				World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, Params);
				FQuat SurfaceRotation = HitResult.ImpactNormal.ToOrientationRotator().Quaternion();
				cursorToWorld->SetWorldLocationAndRotation(HitResult.Location, SurfaceRotation);
			}
		}
		else if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			cursorToWorld->SetWorldLocation(TraceHitResult.Location);
			cursorToWorld->SetWorldRotation(CursorR);
		}
	}
}

/*
void ALoDCharacter::DisableCharacterMovement()
{
	GetCharacterMovement()->DisableMovement();
}

void ALoDCharacter::EnableCharacterMovement()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_NavWalking);
}
*/

void ALoDCharacter::PlaySoundEffectLevelUP()
{
	if (SE_LevelUP) UGameplayStatics::PlaySoundAtLocation(this, this->SE_LevelUP, GetActorLocation());
}
