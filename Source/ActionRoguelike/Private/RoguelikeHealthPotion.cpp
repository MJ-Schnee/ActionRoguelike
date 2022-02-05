// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikeHealthPotion.h"

#include "RoguelikePlayerState.h"
#include "RoguelikeAttributeComponent.h"

ARoguelikeHealthPotion::ARoguelikeHealthPotion()
{
	HealAmount = 100.0f;
	Cost = 100.f;
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

	ARoguelikePlayerState* PlayerState = Cast<ARoguelikePlayerState>(InstigatorPawn->GetPlayerState());
	if (!PlayerState)
	{
		return;
	}

	bool bPurchaseSuccessful = PlayerState->AddCredits(-Cost); 
	if (!bPurchaseSuccessful)
	{
		return;
	}
	
	const bool bHealedInstigator = AttributeComponent->ApplyHealthChange(this, HealAmount);
	if (bHealedInstigator)
	{
		ActivateItemCooldown();
	}
	else
	{
		// Return the player's money if they weren't healed
		PlayerState->AddCredits(Cost);
	}
}
