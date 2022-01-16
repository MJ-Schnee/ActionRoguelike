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
	float OldHealth = Health;

	Health = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);

	float TrueDelta = Health - OldHealth;

	// FIX ME: InstigatorActor parameter is nullptr
	OnHealthChanged.Broadcast(nullptr, this, Health, TrueDelta);

	return !FMath::IsNearlyZero(TrueDelta);
}

bool URoguelikeAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}
