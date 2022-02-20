// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikeActionEffect_Thorns.h"

#include "RoguelikeActionComponent.h"
#include "RoguelikeAttributeComponent.h"

URoguelikeActionEffect_Thorns::URoguelikeActionEffect_Thorns()
{
	// Effect is endless but is activated by an event 
	Duration = 0.0f;
	Period = 0.0f;

	PercentDamageReflected = 0.2f;
}

void URoguelikeActionEffect_Thorns::OnHealthChanged(AActor* InstigatorActor, URoguelikeAttributeComponent* OwningComp,
                                                    float NewHealth, float Delta)
{
	if (InstigatorActor == OwningComp->GetOwner())
	{
		return;
	}

	URoguelikeAttributeComponent* InstigatorAttributeComp =
		URoguelikeAttributeComponent::GetAttributes(InstigatorActor);
	if (InstigatorAttributeComp)
	{
		int ReflectedDamage = static_cast<int>(Delta * PercentDamageReflected);
		InstigatorAttributeComp->ApplyHealthChange(InstigatorActor, ReflectedDamage);
	}
}

void URoguelikeActionEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	URoguelikeAttributeComponent* AttributeComp = URoguelikeAttributeComponent::GetAttributes(Instigator);
	if (!ensure(AttributeComp))
	{
		return;
	}

	AttributeComp->OnHealthChanged.AddDynamic(this, &URoguelikeActionEffect_Thorns::OnHealthChanged);
}

void URoguelikeActionEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	URoguelikeAttributeComponent* AttributeComp = URoguelikeAttributeComponent::GetAttributes(Instigator);
	if (!ensure(AttributeComp))
	{
		return;
	}

	AttributeComp->OnHealthChanged.RemoveDynamic(this, &URoguelikeActionEffect_Thorns::OnHealthChanged);
}
