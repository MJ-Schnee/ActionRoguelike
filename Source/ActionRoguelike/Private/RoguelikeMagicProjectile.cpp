// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikeMagicProjectile.h"

#include "RoguelikeAttributeComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

ARoguelikeMagicProjectile::ARoguelikeMagicProjectile()
{
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ARoguelikeMagicProjectile::OnActorOverlap);
	SphereComp->OnComponentHit.AddDynamic(this, &ARoguelikeMagicProjectile::OnSphereCompHit);
}

void ARoguelikeMagicProjectile::OnSphereCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this->GetInstigator())
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, this->GetTransform());
	}

	Destroy();
}

void ARoguelikeMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
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
