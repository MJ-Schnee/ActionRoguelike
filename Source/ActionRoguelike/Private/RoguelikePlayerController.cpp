// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikePlayerController.h"

#include "Blueprint/UserWidget.h"

void ARoguelikePlayerController::PauseMenuToggle()
{
	if (PauseMenuInstance && PauseMenuInstance->IsInViewport())
	{
		PauseMenuInstance->RemoveFromParent();
		PauseMenuInstance = nullptr;
		SetShowMouseCursor(false);
		SetInputMode(FInputModeGameOnly());

		return;
	}

	PauseMenuInstance = CreateWidget<UUserWidget>(this, PauseMenuClass);
	if (PauseMenuInstance)
	{
		PauseMenuInstance->AddToViewport(100);
		SetShowMouseCursor(true);
		SetInputMode(FInputModeUIOnly());
	}
}

void ARoguelikePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("PauseMenu", IE_Pressed, this, &ARoguelikePlayerController::PauseMenuToggle);
}

void ARoguelikePlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	OnPawnChanged.Broadcast(InPawn);
}

void ARoguelikePlayerController::BeginPlayingState()
{
	BlueprintBeginPlayingState();
}
