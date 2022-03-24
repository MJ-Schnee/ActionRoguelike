// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikeMagicProjectile.h"

#include "RoguelikeActionComponent.h"
#include "RoguelikeGameplayFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ARoguelikeMagicProjectile::ARoguelikeMagicProjectile()
{
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ARoguelikeMagicProjectile::OnActorOverlap);

	MovementComp->InitialSpeed = 2500.0f;

	FlightSoundComp = CreateDefaultSubobject<UAudioComponent>("FlightSoundComp");
	FlightSoundComp->SetupAttachment(SphereComp);

	CameraShakeInnerRadius = 100.0f;
	CameraShakeOuterRadius = 800.0f;

	InitialLifeSpan = 10.0f;

	Damage = 20.f;
}

void ARoguelikeMagicProjectile::Explode_Implementation()
{
	if (ensure(!IsPendingKill()))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, GetActorLocation());

		FlightSoundComp->Stop();
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, GetActorLocation(), GetActorRotation());

		UGameplayStatics::PlayWorldCameraShake(GetWorld(), CameraShake, GetActorLocation(), CameraShakeInnerRadius,
		                                       CameraShakeOuterRadius);

		Destroy();
	}
}

void ARoguelikeMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                               const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		URoguelikeActionComponent* ActionComp = Cast<URoguelikeActionComponent>(
			OtherActor->GetComponentByClass(URoguelikeActionComponent::StaticClass()));
		if (ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
		{
			MovementComp->Velocity = -MovementComp->Velocity;

			SetInstigator(Cast<APawn>(OtherActor));
			return;
		}

		if (URoguelikeGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, Damage, SweepResult))
		{
			Explode();

			if (ActionComp && HasAuthority())
			{
				ActionComp->AddAction(GetInstigator(), DamageEffectClass);
			}
		}
	}
}
