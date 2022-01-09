// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RoguelikeProjectile.h"
#include "RoguelikeMagicProjectile.generated.h"


UCLASS()
class ACTIONROGUELIKE_API ARoguelikeMagicProjectile : public ARoguelikeProjectile
{
	GENERATED_BODY()

public:
	ARoguelikeMagicProjectile();

protected:
	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* HitEffect;
	
	UFUNCTION()
	void OnSphereCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
						 FVector NormalImpulse, const FHitResult& Hit);
	
	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
};
