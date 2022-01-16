// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/RoguelikeAIController.h"

void ARoguelikeAIController::BeginPlay()
{
	Super::BeginPlay();

	if (ensureMsgf(BehaviorTree, TEXT("Behavior Tree is nullptr! Please assign BehaviorTree in AI Controller.")))
	{
		RunBehaviorTree(BehaviorTree);	
	}
}
