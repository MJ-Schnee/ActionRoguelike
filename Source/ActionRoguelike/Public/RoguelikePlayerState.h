// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RoguelikePlayerState.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ARoguelikePlayerState : public APlayerState
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Credits")
	float Credits;

public:

	ARoguelikePlayerState();

	UFUNCTION(BlueprintCallable)
	bool AddCredits(float AdditionalCredits);

	UFUNCTION(BlueprintCallable)
	float GetCredits();
};
