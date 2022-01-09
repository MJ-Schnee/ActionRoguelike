// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RoguelikeProjectile.h"
#include "RoguelikeMagicProjectile.generated.h"


UCLASS()
class ACTIONROGUELIKE_API ARoguelikeMagicProjectile : public ARoguelikeProjectile
{
	GENERATED_BODY()

public:
	ARoguelikeMagicProjectile();

protected:
	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
};
