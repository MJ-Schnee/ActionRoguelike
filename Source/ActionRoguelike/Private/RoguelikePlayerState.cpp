// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikePlayerState.h"

void ARoguelikePlayerState::AddCredits(int32 Delta)
{
	if (!ensure(Delta > 0))
	{
		return;
	}
	
	Credits += Delta;

	OnCreditsChanged.Broadcast(this, Credits, Delta);
}

bool ARoguelikePlayerState::RemoveCredits(int32 Delta)
{
	if (!ensure(Delta > 0))
	{
		return false;
	}

	if (Credits < Delta)
	{
		return false;
	}
	
	Credits -= Delta;

	OnCreditsChanged.Broadcast(this, Credits, -Delta);

	return true;
}

int32 ARoguelikePlayerState::GetCredits() const
{
	return Credits;
}