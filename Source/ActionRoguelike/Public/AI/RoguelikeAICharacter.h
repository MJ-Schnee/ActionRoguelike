// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RoguelikeAICharacter.generated.h"

class URoguelikeAttributeComponent;
class UPawnSensingComponent;
class URoguelikeWorldUserWidget;
class URoguelikeActionComponent;

UCLASS()
class ACTIONROGUELIKE_API ARoguelikeAICharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	URoguelikeAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	URoguelikeActionComponent* ActionComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	UPROPERTY()
	URoguelikeWorldUserWidget* ActiveHealthBar;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName TargetActorKey;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> SeenPlayerWidgetClass;

	UPROPERTY()
	URoguelikeWorldUserWidget* SeenPlayerWidgetInstance;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName IsAggroedKey;

	void SetTargetActor(AActor* NewTarget);

	// Activates the "!" alert widget
	UFUNCTION(NetMulticast, Unreliable)
	void ActivateAlertWidget();

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, URoguelikeAttributeComponent* OwningComp, float NewHealth,
	                     float Delta);

	virtual void PostInitializeComponents() override;

public:
	ARoguelikeAICharacter();
};
