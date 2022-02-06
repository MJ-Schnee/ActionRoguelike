// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RoguelikeAction.generated.h"

UCLASS(Blueprintable)
class ACTIONROGUELIKE_API URoguelikeAction : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);

	// Action nickname to start/stop without referencing object
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;
};
