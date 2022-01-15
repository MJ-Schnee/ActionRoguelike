// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/RoguelikeAIController.h"

#include "BehaviorTree/BlackboardComponent.h"

void ARoguelikeAIController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(BehaviorTree);
	
	APawn* Player = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (Player)
	{
		GetBlackboardComponent()->SetValueAsVector("MoveToLocation", Player->GetActorLocation());
		
		GetBlackboardComponent()->SetValueAsObject("TargetActor", Player);
	}
}
