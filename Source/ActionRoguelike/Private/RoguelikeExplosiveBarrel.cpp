// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikeExplosiveBarrel.h"

#include "RoguelikeAttributeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

ARoguelikeExplosiveBarrel::ARoguelikeExplosiveBarrel()
{
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
	RootComponent = StaticMeshComp;
	StaticMeshComp->SetSimulatePhysics(true);
	StaticMeshComp->SetCollisionProfileName("PhysicsActor");

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>("RadialForceComp");
	RadialForceComp->SetupAttachment(StaticMeshComp);
	RadialForceComp->Radius = 500.0f;
	RadialForceComp->ImpulseStrength = 100000.0f;
}

void ARoguelikeExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	StaticMeshComp->OnComponentHit.AddDynamic(this, &ARoguelikeExplosiveBarrel::OnHit);
}

void ARoguelikeExplosiveBarrel::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	RadialForceComp->FireImpulse();

	if (OtherActor)
	{
		URoguelikeAttributeComponent* AttributeComponent = Cast<URoguelikeAttributeComponent>(
			OtherActor->GetComponentByClass(URoguelikeAttributeComponent::StaticClass()));

		if (AttributeComponent)
		{
			AttributeComponent->ApplyHealthChange(this, -100.0f);
		}
	}

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, this->GetTransform());

	Destroy();
}
