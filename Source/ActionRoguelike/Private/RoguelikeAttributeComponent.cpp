// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikeAttributeComponent.h"

// Sets default values for this component's properties
URoguelikeAttributeComponent::URoguelikeAttributeComponent()
{
	Health = 100.0f;
}

bool URoguelikeAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;

	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);

	return true;
}

