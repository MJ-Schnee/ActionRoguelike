// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RoguelikeAICharacter.generated.h"

class URoguelikeAttributeComponent;
class UPawnSensingComponent;
class URoguelikeWorldUserWidget;

UCLASS()
class ACTIONROGUELIKE_API ARoguelikeAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	ARoguelikeAICharacter();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components");
	URoguelikeAttributeComponent* AttributeComp;

	URoguelikeWorldUserWidget* ActiveHealthBar;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI");
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	virtual void PostInitializeComponents() override;

	void SetTargetActor(AActor* NewTarget);

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, URoguelikeAttributeComponent* OwningComp, float NewHealth, float Delta);
};
