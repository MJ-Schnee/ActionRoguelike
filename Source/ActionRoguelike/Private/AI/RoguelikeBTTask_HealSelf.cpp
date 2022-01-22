// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/RoguelikeBTTask_HealSelf.h"

#include "AIController.h"
#include "RoguelikeAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type URoguelikeBTTask_HealSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (ensure(Controller))
	{
		ACharacter* AICharacter = Cast<ACharacter>(Controller->GetPawn());
		if (AICharacter == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		URoguelikeAttributeComponent* AttributeComp = URoguelikeAttributeComponent::GetAttributes(AICharacter);
		if (ensure(AttributeComp))
		{
			float AIMaxHealth = AttributeComp->GetMaxHealth();
			AttributeComp->ApplyHealthChange(AICharacter, AIMaxHealth);
			
			UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
			BlackboardComp->SetValueAsBool(HideAndHealKey.SelectedKeyName, false);

			return EBTNodeResult::Succeeded;
		}

		return EBTNodeResult::Failed;
	}
	
	return EBTNodeResult::Failed;
}
