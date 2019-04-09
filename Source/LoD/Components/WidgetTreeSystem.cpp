// Fill out your copyright notice in the Description page of Project Settings.

#include "WidgetTreeSystem.h"
#include "UserWidget.h"

// Sets default values for this component's properties
UWidgetTreeSystem::UWidgetTreeSystem()
{
	// Set to false, component will never tick
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UWidgetTreeSystem::PushWidgetIntoTree(TSubclassOf<class UUserWidget> newWidget)
{
	if (newWidget)
	{
		this->widgetArray.Add(newWidget);
	}
}
