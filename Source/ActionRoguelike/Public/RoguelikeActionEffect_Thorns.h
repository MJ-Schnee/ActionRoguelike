// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RoguelikeActionEffect.h"
#include "RoguelikeActionEffect_Thorns.generated.h"

class URoguelikeAttributeComponent;
UCLASS()
class ACTIONROGUELIKE_API URoguelikeActionEffect_Thorns : public URoguelikeActionEffect
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float PercentDamageReflected;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void OnHealthChanged(AActor* InstigatorActor, URoguelikeAttributeComponent* OwningComp,
	                     float NewHealth, float Delta);

public:
	void StartAction_Implementation(AActor* Instigator) override;

	void StopAction_Implementation(AActor* Instigator) override;

	URoguelikeActionEffect_Thorns();
};
