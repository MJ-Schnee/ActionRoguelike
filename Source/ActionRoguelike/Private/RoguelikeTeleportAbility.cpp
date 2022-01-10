// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikeTeleportAbility.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ARoguelikeTeleportAbility::ARoguelikeTeleportAbility()
{
	ExplosionDelay = 0.2f;
	TeleportDelay = 0.2f;
	
	MovementComp->InitialSpeed = 10000.0f;
}

void ARoguelikeTeleportAbility::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	GetWorldTimerManager().SetTimer(ExplosionEffectTimerHandle, this,
		&ARoguelikeTeleportAbility::Explode, ExplosionDelay);
}

void ARoguelikeTeleportAbility::OnActorHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != this->GetInstigator())
	{
		GetWorldTimerManager().ClearTimer(ExplosionEffectTimerHandle);
		Explode();	
	}
}

void ARoguelikeTeleportAbility::Explode_Implementation()
{
	EffectComp->DeactivateSystem();
	MovementComp->StopMovementImmediately();
	SetActorEnableCollision(false);
	
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, this->GetTransform());
	
	FTimerHandle TeleportPlayerTimerHandle;
	GetWorldTimerManager().SetTimer(TeleportPlayerTimerHandle, this,
		&ARoguelikeTeleportAbility::TeleportInstigator, TeleportDelay);
}

void ARoguelikeTeleportAbility::TeleportInstigator()
{
	if(ensure(!IsPendingKill()))
	{
		APawn* InstigatorPawn = GetInstigator();
		if (InstigatorPawn)
		{
			InstigatorPawn->TeleportTo(GetActorLocation(), InstigatorPawn->GetActorRotation());	
		}
	
		Destroy();	
	}
}
