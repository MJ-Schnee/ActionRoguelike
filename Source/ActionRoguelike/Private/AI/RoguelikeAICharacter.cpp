// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/RoguelikeAICharacter.h"

#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "RoguelikeAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
ARoguelikeAICharacter::ARoguelikeAICharacter()
{
 	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");
	
	AttributeComp = CreateDefaultSubobject<URoguelikeAttributeComponent>(TEXT("AttributeComp"));

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ARoguelikeAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &ARoguelikeAICharacter::OnPawnSeen);

	AttributeComp->OnHealthChanged.AddDynamic(this, &ARoguelikeAICharacter::OnHealthChanged);
}

void ARoguelikeAICharacter::OnPawnSeen(APawn* Pawn)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		UBlackboardComponent* BBComp = AIController->GetBlackboardComponent();

		BBComp->SetValueAsObject("TargetActor", Pawn);
	}
}

void ARoguelikeAICharacter::OnHealthChanged(AActor* InstigatorActor, URoguelikeAttributeComponent* OwningComp,
	float NewHealth, float Delta)
{
	// fixme: Placeholder code
	if (NewHealth <= 0.0f)
	{
		Destroy();
	}
}
