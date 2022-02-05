// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RoguelikePickUp.h"
#include "RoguelikeCoinPickUp.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ARoguelikeCoinPickUp : public ARoguelikePickUp
{
	GENERATED_BODY()

public:

	ARoguelikeCoinPickUp();
	
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Credits")
	float CreditWorth;

public:
	
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	
	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
