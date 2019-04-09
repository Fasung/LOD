// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TargetingSystemNPC.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LOD_API UTargetingSystemNPC : public UActorComponent
{
	GENERATED_BODY()
private:

	UPROPERTY()
	class ALoDCharacter * target;

public:	
	// Sets default values for this component's properties
	UTargetingSystemNPC();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE void SetTarget(class ALoDCharacter * newTarget)				{ target = newTarget; }
	
	FORCEINLINE class ALoDCharacter * GetTarget() const						{ return target; }

	FORCEINLINE void ClearTarget()											{ target = NULL; }

	UFUNCTION(BlueprintCallable, Category = "TargetingSystem")
	void ApplyDamage();
};

