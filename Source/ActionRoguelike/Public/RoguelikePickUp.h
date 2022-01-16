// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RoguelikeGameplayInterface.h"
#include "RoguelikePickUp.generated.h"

UCLASS(Abstract)
class ACTIONROGUELIKE_API ARoguelikePickUp : public AActor, public IRoguelikeGameplayInterface
{
	GENERATED_BODY()
	
public:	
	
	ARoguelikePickUp();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;

	FTimerHandle RespawnTimerHandle;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Respawn")
	float RespawnTime;

	virtual void SetItemVisible(bool Visible);

	virtual void ActivateItemCooldown();

	UFUNCTION()
	virtual void RespawnItem();
};
