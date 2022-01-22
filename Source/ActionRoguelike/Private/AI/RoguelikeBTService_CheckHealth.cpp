// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/RoguelikeBTService_CheckHealth.h"

#include "AIController.h"
#include "RoguelikeAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

URoguelikeBTService_CheckHealth::URoguelikeBTService_CheckHealth()
{
	LowHealthPercentage = 0.2f;
}

void URoguelikeBTService_CheckHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackboardComp))
	{
		AAIController* AIController = OwnerComp.GetAIOwner();
		if (ensure(AIController))
		{
			APawn* AIPawn = AIController->GetPawn();
			if (ensure(AIPawn))
			{
				URoguelikeAttributeComponent* AttributeComp = URoguelikeAttributeComponent::GetAttributes(AIPawn);
				if (ensure(AttributeComp))
				{
					// Is AI low enough health?
					float AICurrentHealth = AttributeComp->GetHealth();
					float AIMaxHealth = AttributeComp->GetMaxHealth();
					if (AICurrentHealth / AIMaxHealth <= LowHealthPercentage)
					{
						BlackboardComp->SetValueAsBool(HideAndHealKey.SelectedKeyName, true);
					}
				}
			}
		}
	}
}
