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

	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ensure(AIPawn))
	{
		URoguelikeAttributeComponent* AttributeComp = URoguelikeAttributeComponent::GetAttributes(AIPawn);
		if (ensure(AttributeComp))
		{
			bool bHasLowHealth = AttributeComp->GetHealth() / AttributeComp->GetMaxHealth() <= LowHealthPercentage;
			if (bHasLowHealth)
			{
				UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
				BlackboardComp->SetValueAsBool(HideAndHealKey.SelectedKeyName, true);
			}
		}
	}
}
