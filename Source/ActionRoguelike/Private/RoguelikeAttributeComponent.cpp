// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikeAttributeComponent.h"

#include "RoguelikeCharacter.h"
#include "RoguelikeGameModeBase.h"
#include "AI/RoguelikeAICharacter.h"
#include "Net/UnrealNetwork.h"


static TAutoConsoleVariable<float> CVarAIDamageMultiplier(TEXT("rl.AIDamageMultiplier"), 1.f,
                                                          TEXT("Multiplies damage done by AI in attribute component."),
                                                          ECVF_Cheat);

static TAutoConsoleVariable<float> CVarPlayerDamageMultiplier(TEXT("rl.PlayerDamageMultiplier"), 1.f,
                                                              TEXT(
	                                                              "Multiplies damage done by Players in attribute component."),
                                                              ECVF_Cheat);

// Sets default values for this component's properties
URoguelikeAttributeComponent::URoguelikeAttributeComponent()
{
	Health = 100.0f;
	MaxHealth = 100.0f;

	Rage = 0.0f;
	MaxRage = 100.0f;
	RageMultiplier = 0.2f;

	SetIsReplicatedByDefault(true);
}

URoguelikeAttributeComponent* URoguelikeAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<URoguelikeAttributeComponent>(
			FromActor->GetComponentByClass(URoguelikeAttributeComponent::StaticClass()));
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

	// Damage scaling
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
	}

	float OldHealth = Health;
	float NewHealth = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);
	float HealthDelta = NewHealth - OldHealth;

	// Only server will actually change values
	if (GetOwner()->HasAuthority())
	{
		// Change health
		if (!FMath::IsNearlyZero(HealthDelta))
		{
			Health = NewHealth;
			MulticastHealthChanged(InstigatorActor, NewHealth, HealthDelta);
		}

		// Change rage
		if (Delta <= 0.0f)
		{
			float NewRage = static_cast<int>(FMath::Clamp(Rage + FMath::Abs(Delta) * RageMultiplier, 0.0f, MaxRage));
			if (GetOwner()->HasAuthority())
			{
				Rage = NewRage;
				MulticastRageChanged(this, Rage);
			}
		}

		// Died
		if (HealthDelta < 0.f && FMath::IsNearlyZero(NewHealth))
		{
			ARoguelikeGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ARoguelikeGameModeBase>();
			if (GameMode)
			{
				GameMode->OnActorKilled(GetOwner(), InstigatorActor);
			}
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

void URoguelikeAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth,
                                                                         float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta);
}

void URoguelikeAttributeComponent::MulticastRageChanged_Implementation(URoguelikeAttributeComponent* OwningComp,
                                                                       int NewRage)
{
	OnRageChanged.Broadcast(OwningComp, NewRage);
}

void URoguelikeAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URoguelikeAttributeComponent, Health);
	DOREPLIFETIME(URoguelikeAttributeComponent, MaxHealth);
	DOREPLIFETIME(URoguelikeAttributeComponent, Rage);
	DOREPLIFETIME(URoguelikeAttributeComponent, MaxRage);
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
