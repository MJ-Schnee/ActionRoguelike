// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikeHealthPotion.h"

#include "RoguelikePlayerState.h"
#include "RoguelikeAttributeComponent.h"

#define LOCTEXT_NAMESPACE "InteractableActors"

ARoguelikeHealthPotion::ARoguelikeHealthPotion()
{
	HealAmount = 100.0f;
	Cost = 100;
}

FText ARoguelikeHealthPotion::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	URoguelikeAttributeComponent* AttributeComponent = Cast<URoguelikeAttributeComponent>(
		InstigatorPawn->GetComponentByClass(URoguelikeAttributeComponent::StaticClass()));
	if (AttributeComponent && AttributeComponent->GetHealth() == AttributeComponent->GetMaxHealth())
	{
		return NSLOCTEXT(LOCTEXT_NAMESPACE, "HealthPotion_FullHealthWarning", "Already at full health.");
	}

	return FText::Format(NSLOCTEXT(LOCTEXT_NAMESPACE, "HealthPotion_InteractMessage",
	                               "Costs {0} credits. Restores {1} HP."), Cost, HealAmount);
}

void ARoguelikeHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!InstigatorPawn)
	{
		return;
	}

	URoguelikeAttributeComponent* AttributeComponent = Cast<URoguelikeAttributeComponent>(
		InstigatorPawn->GetComponentByClass(URoguelikeAttributeComponent::StaticClass()));
	if (!AttributeComponent)
	{
		return;
	}

	ARoguelikePlayerState* PlayerState = InstigatorPawn->GetPlayerState<ARoguelikePlayerState>();
	if (!PlayerState)
	{
		return;
	}

	bool bHasEnoughCredits = PlayerState->GetCredits() >= Cost;
	if (!bHasEnoughCredits)
	{
		return;
	}

	const bool bHealedInstigator = AttributeComponent->ApplyHealthChange(this, HealAmount);
	if (bHealedInstigator)
	{
		ActivateItemCooldown();
		PlayerState->RemoveCredits(Cost);
	}
}

#undef LOCTEXT_NAMESPACE
