// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/RoguelikeAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "DrawDebugHelpers.h"
#include "RoguelikeAttributeComponent.h"
#include "RoguelikeWorldUserWidget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
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

void ARoguelikeAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIController = GetController<AAIController>();
	if (AIController)
	{
		AIController->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);
	}
}

void ARoguelikeAICharacter::OnPawnSeen(APawn* Pawn)
{
	SetTargetActor(Pawn);
}

void ARoguelikeAICharacter::OnHealthChanged(AActor* InstigatorActor, URoguelikeAttributeComponent* OwningComp,
	float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		if (ActiveHealthBar == nullptr)
		{
			ActiveHealthBar = CreateWidget<URoguelikeWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if (ActiveHealthBar)
			{
				ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport();
			}	
		}
		
		if (NewHealth >= 0.0f)
		{
			GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
		}

		if (NewHealth <= 0.0f)
		{
			if (InstigatorActor != this)
			{
				SetTargetActor(InstigatorActor);
			}
		
			AAIController* AIController = GetController<AAIController>();
			if (AIController)
			{
				AIController->GetBrainComponent()->StopLogic("Killed");
			}

			GetMesh()->SetCollisionProfileName("Ragdoll");
			GetMesh()->SetAllBodiesSimulatePhysics(true);
		
			SetLifeSpan(10.0f);
		}
	}
}
