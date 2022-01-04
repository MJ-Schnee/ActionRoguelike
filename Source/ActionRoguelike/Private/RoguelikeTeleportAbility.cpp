// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikeTeleportAbility.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ARoguelikeTeleportAbility::ARoguelikeTeleportAbility()
{
	MovementComp->InitialSpeed = 10000.0f;
	
	SphereComp->OnComponentHit.AddDynamic(this, &ARoguelikeTeleportAbility::OnSphereCompHit);
}

void ARoguelikeTeleportAbility::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(ExplosionEffectTimerHandle, this,
		&ARoguelikeTeleportAbility::TriggerExplosionEffect, 0.2f, false);
}

void ARoguelikeTeleportAbility::OnSphereCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != this->GetInstigator())
	{
		GetWorldTimerManager().ClearTimer(ExplosionEffectTimerHandle);
		TriggerExplosionEffect();	
	}
}

void ARoguelikeTeleportAbility::TriggerExplosionEffect()
{
	MovementComp->StopMovementImmediately();

	EffectComp->Deactivate();
	
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, this->GetTransform());
	
	GetWorldTimerManager().SetTimer(TeleportPlayerTimerHandle, this,
		&ARoguelikeTeleportAbility::TeleportPlayer, 0.2f, false);
}

void ARoguelikeTeleportAbility::TeleportPlayer()
{
	APawn* InstigatorPawn = GetInstigator();
	InstigatorPawn->TeleportTo(this->GetTransform().GetLocation(), InstigatorPawn->GetActorRotation());
	
	Destroy();
}
