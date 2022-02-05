// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikePlayerState.h"

ARoguelikePlayerState::ARoguelikePlayerState()
{
	Credits = 0.f;
}

bool ARoguelikePlayerState::AddCredits(float AdditionalCredits)
{
	float newCreditAmount = Credits + AdditionalCredits;
	
	if (newCreditAmount < 0.f)
	{
		return false;
	}

	Credits = newCreditAmount;
	
	return true;
}

float ARoguelikePlayerState::GetCredits()
{
	return Credits;
}