// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RoguelikeAttributeComponent.generated.h"

class URoguelikeAttributeComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, URoguelikeAttributeComponent*,
	OwningComp, float, NewHealth, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API URoguelikeAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	URoguelikeAttributeComponent();

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static URoguelikeAttributeComponent* GetAttributes(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category = "Attributes", meta = (DisplayName = "IsAlive"))
	static bool IsActorAlive(AActor* Actor);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float MaxHealth;

public:

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	// Add/subtract a specified amount of health to the actor, returns action success
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;
};
