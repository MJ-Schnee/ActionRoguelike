// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikeCoinPickUp.h"

#include "RoguelikeCharacter.h"
#include "RoguelikePlayerState.h"

ARoguelikeCoinPickUp::ARoguelikeCoinPickUp()
{
	CreditWorth = 15.f;

	MeshComp->OnComponentBeginOverlap.AddDynamic(this, &ARoguelikeCoinPickUp::OnActorOverlap);
}

void ARoguelikeCoinPickUp::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!InstigatorPawn)
	{
		return;
	}

	ARoguelikePlayerState* PlayerState = Cast<ARoguelikePlayerState>(InstigatorPawn->GetPlayerState());
	if (!PlayerState)
	{
		return;
	}

	if (PlayerState->AddCredits(CreditWorth))
	{
		ActivateItemCooldown();
	}
}

void ARoguelikeCoinPickUp::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARoguelikeCharacter* Player = Cast<ARoguelikeCharacter>(OtherActor);
	if (Player)
	{
		Interact_Implementation(Cast<APawn>(OtherActor));
	}
}
