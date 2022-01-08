// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RoguelikeAttributeComponent.h"
#include "GameFramework/Actor.h"
#include "RoguelikeTargetDummy.generated.h"

class URoguelikeAttributeComponent;

UCLASS()
class ACTIONROGUELIKE_API ARoguelikeTargetDummy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoguelikeTargetDummy();

protected:
	UPROPERTY(VisibleAnywhere)
	URoguelikeAttributeComponent* AttributeComp;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, URoguelikeAttributeComponent* OwningComp, float NewHealth, float Delta);

};
