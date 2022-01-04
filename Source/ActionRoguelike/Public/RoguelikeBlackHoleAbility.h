// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RoguelikeProjectile.h"
#include "RoguelikeBlackHoleAbility.generated.h"

class URadialForceComponent;

/**
 * Black hole ability used by player
 * Creates a black hole that moves forward and sucks in all objects
 * When the objects overlap the black hole, they are destroyed (this logic is located in blueprints)
 */
UCLASS()
class ACTIONROGUELIKE_API ARoguelikeBlackHoleAbility : public ARoguelikeProjectile
{
	GENERATED_BODY()
	
public:	
	ARoguelikeBlackHoleAbility();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	URadialForceComponent* RadialForceComp;
};
