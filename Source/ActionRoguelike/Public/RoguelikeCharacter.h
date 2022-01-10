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

UCLASS()
class ACTIONROGUELIKE_API ARoguelikeCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	
	// Primary attack
	UPROPERTY(EditAnywhere, Category="Attack")
	TSubclassOf<ARoguelikeProjectile> PrimaryAttackBP;

	UPROPERTY(EditAnywhere, Category="Attack")
	UAnimMontage* PrimaryAttackAnim;

	FTimerDelegate TimerDelegate_PrimaryAttack;

	// Black hole ability
	UPROPERTY(EditAnywhere, Category="Attack")
	TSubclassOf<ARoguelikeProjectile> BlackHoleAbilityBP;
	
	UPROPERTY(EditAnywhere, Category="Attack")
	UAnimMontage* BlackHoleAbilityAnim;
	
	FTimerDelegate TimerDelegate_BlackHoleAbility;

	// Teleport ability
	UPROPERTY(EditAnywhere, Category="Attack")
	TSubclassOf<ARoguelikeProjectile> TeleportAbilityBP;
	
	UPROPERTY(EditAnywhere, Category="Attack")
	UAnimMontage* TeleportAbilityAnim;
	
	FTimerDelegate TimerDelegate_TeleportAbility;

public:
	
	// Sets default values for this character's properties
	ARoguelikeCharacter();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components");
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components");
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components");
	URoguelikeInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components");
	URoguelikeAttributeComponent* AttributeComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects");
	UParticleSystem* MuzzleFlash;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called after constructor but right before BeginPlay()
	virtual void PostInitializeComponents() override;

	void MoveForward(float Value);
	
	void MoveRight(float Value);
	
	void PrimaryInteract();

	// Function bound to timers to spawn player abilities
	UFUNCTION()
	void UseAbility_TimeElapsed(FName SpawnSocket, UClass* AbilityClass);

	void PrimaryAttack();
	
	void BlackHoleAbility();

	void TeleportAbility();

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, URoguelikeAttributeComponent* OwningComp, float NewHealth, float Delta);

public:
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
