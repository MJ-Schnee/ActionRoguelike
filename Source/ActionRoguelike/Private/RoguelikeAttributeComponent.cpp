// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikeAttributeComponent.h"

// Sets default values for this component's properties
URoguelikeAttributeComponent::URoguelikeAttributeComponent()
{
	Health = 100.0f;
	MaxHealth = 100.0f;
}

bool URoguelikeAttributeComponent::ApplyHealthChange(float Delta)
{
	if (FMath::IsNearlyEqual(Health, MaxHealth) && Delta > 0.0f)
	{
		return false;
	}
	
	if (FMath::IsNearlyEqual(Health, 0.0f) && Delta < 0.0f)
	{
		return false;
	}
	
	Health += Delta;

	if (Health < 0.0f)
	{
		Health = 0.0f;
	}
	else if (Health > MaxHealth)
	{
		Health = MaxHealth;
	}

	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);

	return true;
}

bool URoguelikeAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}
