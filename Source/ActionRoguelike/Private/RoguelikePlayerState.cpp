// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikePlayerState.h"

#include "Net/UnrealNetwork.h"

void ARoguelikePlayerState::MulticastCreditsChanged_Implementation(ARoguelikePlayerState* PlayerState, int32 NewCredits,
                                                                   int32 Delta)
{
	OnCreditsChanged.Broadcast(PlayerState, NewCredits, Delta);
}

void ARoguelikePlayerState::AddCredits(int32 Delta)
{
	if (!ensure(Delta > 0))
	{
		return;
	}

	Credits += Delta;

	MulticastCreditsChanged(this, Credits, Delta);

	if (!HasAuthority())
	{
		UE_LOG(LogTemp, Log, TEXT("Client credits: %f"), Credits);
	}
}

bool ARoguelikePlayerState::RemoveCredits(int32 Delta)
{
	if (!ensure(Delta >= 0))
	{
		return false;
	}

	if (Credits < Delta)
	{
		return false;
	}

	Credits -= Delta;

	MulticastCreditsChanged(this, Credits, Delta);

	return true;
}

int32 ARoguelikePlayerState::GetCredits() const
{
	return Credits;
}

void ARoguelikePlayerState::SavePlayerState_Implementation(URoguelikeSaveGame* SaveObject)
{
	if (SaveObject)
	{
		SaveObject->Credits = Credits;
	}
}

void ARoguelikePlayerState::LoadPlayerState_Implementation(URoguelikeSaveGame* SaveObject)
{
	if (SaveObject)
	{
		Credits = SaveObject->Credits;
	}
}

void ARoguelikePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARoguelikePlayerState, Credits);
}
