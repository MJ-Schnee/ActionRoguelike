// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/RoguelikeAICharacter.h"

// Sets default values
ARoguelikeAICharacter::ARoguelikeAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARoguelikeAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARoguelikeAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

