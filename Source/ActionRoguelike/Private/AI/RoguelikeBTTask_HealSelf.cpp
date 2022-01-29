// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/RoguelikeBTTask_HealSelf.h"

#include "AIController.h"
#include "RoguelikeAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type URoguelikeBTTask_HealSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACharacter* AICharacter = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (AICharacter == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	URoguelikeAttributeComponent* AttributeComp = URoguelikeAttributeComponent::GetAttributes(AICharacter);
	if (ensure(AttributeComp))
	{
		AttributeComp->ApplyHealthChange(AICharacter, AttributeComp->GetMaxHealth());

		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
