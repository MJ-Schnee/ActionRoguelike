// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RoguelikePickUp.h"
#include "RoguelikeHealthPotion.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ARoguelikeHealthPotion : public ARoguelikePickUp
{
	GENERATED_BODY()

public:
	ARoguelikeHealthPotion();

	virtual FText GetInteractText_Implementation(APawn* InstigatorPawn) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
	float HealAmount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Credits")
	int32 Cost;

public:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
};
