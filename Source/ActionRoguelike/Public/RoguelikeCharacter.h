// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RoguelikeCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class URoguelikeInteractionComponent;
class UAnimMontage;
class ARoguelikeProjectile;
class URoguelikeAttributeComponent;
class URoguelikeActionComponent;

UCLASS()
class ACTIONROGUELIKE_API ARoguelikeCharacter : public ACharacter
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	URoguelikeInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	URoguelikeAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	URoguelikeActionComponent* ActionComp;

	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	// 3rd person perspective means we need to change where the "eyes" are by default
	virtual FVector GetPawnViewLocation() const override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void SprintStart();

	void SprintStop();

	void PrimaryInteract();

	void PrimaryAttack();

	void BlackHoleAbility();

	void TeleportAbility();

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, URoguelikeAttributeComponent* OwningComp, float NewHealth,
	                     float Delta);

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Sets default values for this character's properties
	ARoguelikeCharacter();

};
