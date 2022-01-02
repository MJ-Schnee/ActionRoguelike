// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RoguelikeCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class URoguelikeInteractionComponent;
class UAnimMontage;
class ARoguelikeProjectileAbility;

UCLASS()
class ACTIONROGUELIKE_API ARoguelikeCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	// Primary attack
	UPROPERTY(EditAnywhere, Category="Attack")
	TSubclassOf<ARoguelikeProjectileAbility> PrimaryAttackBP;

	UPROPERTY(EditAnywhere, Category="Attack")
	UAnimMontage* PrimaryAttackAnim;

	FTimerDelegate TimerDelegate_PrimaryAttack;
	FTimerHandle TimerHandle_PrimaryAttack;

	// Black hole ability
	UPROPERTY(EditAnywhere, Category="Attack")
	TSubclassOf<ARoguelikeProjectileAbility> BlackHoleAbilityBP;
	
	UPROPERTY(EditAnywhere, Category="Attack")
	UAnimMontage* BlackHoleAbilityAnim;
	
	FTimerDelegate TimerDelegate_BlackHoleAbility;
	FTimerHandle TimerHandle_BlackHoleAbility;

public:
	// Sets default values for this character's properties
	ARoguelikeCharacter();

protected:

	UPROPERTY(VisibleAnywhere);
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere);
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere);
	URoguelikeInteractionComponent* InteractionComp;
	
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

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
