// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/RoguelikeAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "DrawDebugHelpers.h"
#include "RoguelikeActionComponent.h"
#include "RoguelikeAttributeComponent.h"
#include "RoguelikeWorldUserWidget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
ARoguelikeAICharacter::ARoguelikeAICharacter()
{
 	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");
	
	AttributeComp = CreateDefaultSubobject<URoguelikeAttributeComponent>(TEXT("AttributeComp"));

	ActionComp = CreateDefaultSubobject<URoguelikeActionComponent>(TEXT("ActionComp"));

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
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

		// Killed
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

			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();
		
			SetLifeSpan(10.0f);
		}
	}
}
