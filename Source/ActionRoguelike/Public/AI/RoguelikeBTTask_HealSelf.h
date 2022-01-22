// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RoguelikeBTTask_HealSelf.generated.h"

UCLASS()
class ACTIONROGUELIKE_API URoguelikeBTTask_HealSelf : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	
	// Whether or not the bot should hide and heal
	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector HideAndHealKey;
	
};
