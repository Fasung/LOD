// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WidgetTreeSystem.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LOD_API UWidgetTreeSystem : public UActorComponent
{
	GENERATED_BODY()

protected:	

	UPROPERTY()
	TArray<class TSubclassOf<class UUserWidget>> widgetArray;
	
public:


	/** Assign new blueprint widget */
	UFUNCTION(BlueprintCallable, Category = "WidgetTreeSystemUtilityFunctions")
	void PushWidgetIntoTree(TSubclassOf<class UUserWidget> newWidget);

public:
	// Sets default values for this component's properties
	UWidgetTreeSystem();

};
