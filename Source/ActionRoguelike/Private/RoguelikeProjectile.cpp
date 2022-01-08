// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikeProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

ARoguelikeProjectile::ARoguelikeProjectile()
{
	// To improve performance, disable Tick() by default
	PrimaryActorTick.bCanEverTick = false;
	
 	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	RootComponent = SphereComp;
	SphereComp->SetCollisionProfileName("Projectile");

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 1000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
	MovementComp->ProjectileGravityScale = 0.0f;
}

void ARoguelikeProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	SphereComp->IgnoreActorWhenMoving(this->GetInstigator(), true);
}