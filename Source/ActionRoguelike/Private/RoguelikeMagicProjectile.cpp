// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikeMagicProjectile.h"

#include "RoguelikeAttributeComponent.h"
#include "Components/SphereComponent.h"

ARoguelikeMagicProjectile::ARoguelikeMagicProjectile()
{
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ARoguelikeMagicProjectile::OnActorOverlap);
}

void ARoguelikeMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		URoguelikeAttributeComponent* AttributeComponent = Cast<URoguelikeAttributeComponent>(
			OtherActor->GetComponentByClass(URoguelikeAttributeComponent::StaticClass()));

		if (AttributeComponent)
		{
			AttributeComponent->ApplyHealthChange(-20.0f);

			Destroy();
		}
	}
}
