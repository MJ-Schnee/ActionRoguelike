// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikePlayerController.h"

void ARoguelikePlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	OnPawnChanged.Broadcast(InPawn);
}

void ARoguelikePlayerController::BeginPlayingState()
{
	BluepringBeginPlayingState();
}
