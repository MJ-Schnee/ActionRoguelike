// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikeItemChest.h"

// Sets default values
ARoguelikeItemChest::ARoguelikeItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LidMesh->SetupAttachment(BaseMesh);

	TargetPitch = 110;
}

// Called when the game starts or when spawned
void ARoguelikeItemChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARoguelikeItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

