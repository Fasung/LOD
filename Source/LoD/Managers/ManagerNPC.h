// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LoDEnumTypes.h"
#include "Components/ActorComponent.h"
#include "ManagerNPC.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LOD_API UManagerNPC : public UActorComponent
{
	GENERATED_BODY()

private: /** Properties */

	/**  */
	UPROPERTY()	TArray<class ANPC * > npcArrayLVL2;

	/**  */
	UPROPERTY()	TArray<class ANPC * > npcArrayLVL3_Part1;

	/**  */
	UPROPERTY()	TArray<class ANPC * > npcArrayLVL3_Part2;

	/**  */
	UPROPERTY()	TArray<class ANPC * > npcArrayLVL4_Part1;

	/**  */
	UPROPERTY()	TArray<class ANPC * > npcArrayLVL5;

	/** Used to calculate distance from character to NPC, if distance is too big, do not render widget, otherwise update widget rotation */
	UPROPERTY() FVector distanceToNPC;

	/** */
	UPROPERTY() FVector npcDisplacement;

public:

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public: /** FORCEINLINE Functions*/

	FORCEINLINE void PushNewNPC(class ANPC * newNPC);
	
	FORCEINLINE void RemoveNPC(class ANPC * dyingNPC);

	
	UFUNCTION(BlueprintCallable, Category = "Enable/DissableNPC")
	void DissableNPCAtLevel(ECurrentLocation atLocation);
	
	UFUNCTION(BlueprintCallable, Category = "Enable/DissableNPC")
	void EnableNPCAtLevel(ECurrentLocation atLocation);


private: /** Functions */

	UFUNCTION() void UpdateNPC(TArray<class ANPC * > & npcArrayToUpdate);

	UFUNCTION() void EnableNPC(TArray<class ANPC * > & npcArrayToUpdate);

	UFUNCTION() void DisableNPC(TArray<class ANPC * > & npcArrayToUpdate);

public:
	// Sets default values for this component's properties
	UManagerNPC();

};
