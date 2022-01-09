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
	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float ExplosionDelay;
	
	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float TeleportDelay;
	
	FTimerHandle ExplosionEffectTimerHandle;

	virtual void OnActorHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                FVector NormalImpulse, const FHitResult& Hit) override;
	
	UFUNCTION()
	void TriggerExplosionEffect();
	
	UFUNCTION()
	void TeleportInstigator();
	
	virtual void PostInitializeComponents() override;

public:
	ARoguelikeTeleportAbility();
	
};
