// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikePickUp.h"

ARoguelikePickUp::ARoguelikePickUp()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;
	
	RespawnTime = 10.0f;
}

void ARoguelikePickUp::Interact_Implementation(APawn* InstigatorPawn)
{
	MeshComp->SetVisibility(false);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetWorldTimerManager().SetTimer(RespawnTimerHandle, this,
		&ARoguelikePickUp::RespawnItem, RespawnTime);
}

void ARoguelikePickUp::RespawnItem()
{
	MeshComp->SetVisibility(true);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}
