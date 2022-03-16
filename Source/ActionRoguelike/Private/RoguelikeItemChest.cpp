// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikeItemChest.h"

#include "Net/UnrealNetwork.h"

ARoguelikeItemChest::ARoguelikeItemChest()
{
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LidMesh->SetupAttachment(BaseMesh);

	TargetPitch = 110;

	bReplicates = true;
}

void ARoguelikeItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	bLidOpened = !bLidOpened;
	OnRep_LidOpened();
}

void ARoguelikeItemChest::OnRep_LidOpened()
{
	float CurrPitch = bLidOpened ? TargetPitch : 0.0f;
	LidMesh->SetRelativeRotation(FRotator(CurrPitch, 0, 0));
}

void ARoguelikeItemChest::OnActorLoaded_Implementation()
{
	OnRep_LidOpened();
}

void ARoguelikeItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARoguelikeItemChest, bLidOpened);
}
