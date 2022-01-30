// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikeGameplayFunctionLibrary.h"

#include "RoguelikeAttributeComponent.h"

bool URoguelikeGameplayFunctionLibrary::ApplyDamage(AActor* DamageInstigator, AActor* TargetActor, float DamageAmount)
{
	URoguelikeAttributeComponent* AttributeComp = URoguelikeAttributeComponent::GetAttributes(TargetActor);
	if (AttributeComp)
	{
		return AttributeComp->ApplyHealthChange(DamageInstigator, -DamageAmount);
	}

	return false;
}

bool URoguelikeGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageInstigator, AActor* TargetActor,
	float DamageAmount, const FHitResult& HitResult)
{
	if(ApplyDamage(DamageInstigator, TargetActor, DamageAmount))
	{
		UPrimitiveComponent* HitResultComp = HitResult.GetComponent();
		if (HitResultComp && HitResultComp->IsSimulatingPhysics(HitResult.BoneName))
		{
			HitResultComp->AddImpulseAtLocation(-HitResult.ImpactNormal * 300000.f, HitResult.ImpactPoint, HitResult.BoneName);
		}
		
		return true;
	}

	return false;
}
