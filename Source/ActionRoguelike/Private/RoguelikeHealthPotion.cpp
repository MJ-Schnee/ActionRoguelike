// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikeHealthPotion.h"

#include "RoguelikePlayerState.h"
#include "RoguelikeAttributeComponent.h"

ARoguelikeHealthPotion::ARoguelikeHealthPotion()
{
	HealAmount = 100.0f;
	Cost = 100;
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

	bool bHasEnoughCredits = PlayerState->GetCredits() <= Cost; 
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
