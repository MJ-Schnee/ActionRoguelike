// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikePickUp.h"

ARoguelikePickUp::ARoguelikePickUp()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;
	
	RespawnTime = 10.0f;
}

void ARoguelikePickUp::SetItemVisible(bool Visible)
{
	RootComponent->SetVisibility(Visible, true);
	SetActorEnableCollision(Visible);
}

void ARoguelikePickUp::ActivateItemCooldown()
{
	SetItemVisible(false);
	
	GetWorldTimerManager().SetTimer(RespawnTimerHandle, this,
		&ARoguelikePickUp::RespawnItem, RespawnTime);
}

void ARoguelikePickUp::RespawnItem()
{
	SetItemVisible(true);
}
