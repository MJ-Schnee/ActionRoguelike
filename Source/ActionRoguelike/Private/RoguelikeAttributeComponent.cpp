// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikeAttributeComponent.h"

// Sets default values for this component's properties
URoguelikeAttributeComponent::URoguelikeAttributeComponent()
{
	Health = 100.0f;
	MaxHealth = 100.0f;
}

URoguelikeAttributeComponent* URoguelikeAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<URoguelikeAttributeComponent>(FromActor->GetComponentByClass(URoguelikeAttributeComponent::StaticClass()));
	}
	
	return nullptr;
}

bool URoguelikeAttributeComponent::IsActorAlive(AActor* Actor)
{
	URoguelikeAttributeComponent* AttributeComp = GetAttributes(Actor);
	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}

	return false;
}

float URoguelikeAttributeComponent::GetMaxHealth()
{
	return MaxHealth;
}

float URoguelikeAttributeComponent::GetHealth()
{
	return Health;
}

bool URoguelikeAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged())
	{
		return false;
	}
	
	float OldHealth = Health;

	Health = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);

	float TrueDelta = Health - OldHealth;

	OnHealthChanged.Broadcast(InstigatorActor, this, Health, TrueDelta);

	return !FMath::IsNearlyZero(TrueDelta);
}

bool URoguelikeAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}
