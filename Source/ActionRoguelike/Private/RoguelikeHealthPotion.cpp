// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikeHealthPotion.h"

#include "RoguelikeAttributeComponent.h"

ARoguelikeHealthPotion::ARoguelikeHealthPotion()
{
	HealAmount = 100.0f;
}

void ARoguelikeHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{	
	if (InstigatorPawn)
	{
		URoguelikeAttributeComponent* AttributeComponent = Cast<URoguelikeAttributeComponent>(
			InstigatorPawn->GetComponentByClass(URoguelikeAttributeComponent::StaticClass()));
	
		if (AttributeComponent)
		{
			const bool bHealedInstigator = AttributeComponent->ApplyHealthChange(HealAmount);
	
			if (bHealedInstigator)
			{
				ActivateItemCooldown();
			}
		}
	}
}
