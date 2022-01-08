// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikeTargetDummy.h"

// Sets default values
ARoguelikeTargetDummy::ARoguelikeTargetDummy()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;

	AttributeComp = CreateDefaultSubobject<URoguelikeAttributeComponent>("AttributeComp");
	AttributeComp->OnHealthChanged.AddDynamic(this, &ARoguelikeTargetDummy::OnHealthChanged);
}

void ARoguelikeTargetDummy::OnHealthChanged(AActor* InstigatorActor, URoguelikeAttributeComponent* OwningComp,
	float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		MeshComp->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);	
	}
}
