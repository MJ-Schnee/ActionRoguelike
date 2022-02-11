// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RoguelikeProjectile.h"
#include "RoguelikeMagicProjectile.generated.h"


UCLASS()
class ACTIONROGUELIKE_API ARoguelikeMagicProjectile : public ARoguelikeProjectile
{
	GENERATED_BODY()

public:
	ARoguelikeMagicProjectile();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UAudioComponent* FlightSoundComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	USoundBase* ImpactSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CameraShake")
	TSubclassOf<UCameraShakeBase> CameraShake;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CameraShake")
	float CameraShakeInnerRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CameraShake")
	float CameraShakeOuterRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Action")
	FGameplayTag ParryTag;

	virtual void Explode_Implementation() override;

	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
