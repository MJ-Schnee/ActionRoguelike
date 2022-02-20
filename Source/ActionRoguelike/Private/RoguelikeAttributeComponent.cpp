// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikeAttributeComponent.h"

#include "RoguelikeCharacter.h"
#include "RoguelikeGameModeBase.h"
#include "AI/RoguelikeAICharacter.h"


static TAutoConsoleVariable<float> CVarAIDamageMultiplier(TEXT("rl.AIDamageMultiplier"), 1.f,
	TEXT("Multiplies damage done by AI in attribute component."), ECVF_Cheat);

static TAutoConsoleVariable<float> CVarPlayerDamageMultiplier(TEXT("rl.PlayerDamageMultiplier"), 1.f,
	TEXT("Multiplies damage done by Players in attribute component."), ECVF_Cheat);

// Sets default values for this component's properties
URoguelikeAttributeComponent::URoguelikeAttributeComponent()
{
	Health = 100.0f;
	MaxHealth = 100.0f;
	
	Rage = 0.0f;
	MaxRage = 100.0f;
	RageMultiplier = 0.2f;
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

bool URoguelikeAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!IsAlive() || !GetOwner()->CanBeDamaged() && Delta < 0.f)
	{
		return false;
	}

	if (Delta < 0.f)
	{
		ARoguelikeAICharacter* AICharacter = Cast<ARoguelikeAICharacter>(InstigatorActor);
		if (AICharacter)
		{
			float DamageMultiplier = CVarAIDamageMultiplier.GetValueOnGameThread();
			Delta *= DamageMultiplier;
		}
		
		ARoguelikeCharacter* Player = Cast<ARoguelikeCharacter>(InstigatorActor);
		if (Player)
		{
			float DamageMultiplier = CVarPlayerDamageMultiplier.GetValueOnGameThread();
			Delta *= DamageMultiplier;
		}

		// Add to rage
		Rage = static_cast<int>(FMath::Clamp(Rage + FMath::Abs(Delta) * RageMultiplier, 0.0f, MaxRage));
		OnRageChanged.Broadcast(this, Rage);
	}
	
	float OldHealth = Health;

	Health = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);

	float HealthDelta = Health - OldHealth;

	OnHealthChanged.Broadcast(InstigatorActor, this, Health, HealthDelta);

	// Died
	if (HealthDelta < 0.f && FMath::IsNearlyZero(Health))
	{
		ARoguelikeGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ARoguelikeGameModeBase>();
		if (GameMode)
		{
			GameMode->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}

	return !FMath::IsNearlyZero(HealthDelta);
}

bool URoguelikeAttributeComponent::SubtractRage(float Delta)
{
	if (!ensureAlways(Delta > 0.0f))
	{
		return false;
	}

	if (Rage < Delta)
	{
		return false;
	}

	Rage -= Delta;
	OnRageChanged.Broadcast(this, Rage);
	return true;
}

bool URoguelikeAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

float URoguelikeAttributeComponent::GetMaxHealth()
{
	return MaxHealth;
}

float URoguelikeAttributeComponent::GetHealth()
{
	return Health;
}

float URoguelikeAttributeComponent::GetMaxRage()
{
	return MaxRage;
}

float URoguelikeAttributeComponent::GetRage()
{
	return Rage;
}
