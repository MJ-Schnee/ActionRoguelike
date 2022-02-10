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
			// Direction hit from = Normalize vector of Target location - Origin location
			FVector Direction = HitResult.TraceEnd - HitResult.TraceStart;
			Direction.Normalize();
			
			HitResultComp->AddImpulseAtLocation(Direction * 300000.f, HitResult.ImpactPoint, HitResult.BoneName);
		}
		
		return true;
	}

	return false;
}
