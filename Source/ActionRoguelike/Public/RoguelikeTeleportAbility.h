// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RoguelikeProjectile.h"
#include "RoguelikeTeleportAbility.generated.h"

/**
 * Ability which shoots a projectile that, upon impact or after a set time, teleports the player to the projectile
 */
UCLASS()
class ACTIONROGUELIKE_API ARoguelikeTeleportAbility : public ARoguelikeProjectile
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere);
	UParticleSystem* ExplosionEffect;
	
	FTimerHandle ExplosionEffectTimerHandle;
	FTimerHandle TeleportPlayerTimerHandle;

	UFUNCTION()
	void OnSphereCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	UFUNCTION()
	void TriggerExplosionEffect();
	
	UFUNCTION()
	void TeleportPlayer();
	
	virtual void BeginPlay() override;

public:
	ARoguelikeTeleportAbility();
	
};
