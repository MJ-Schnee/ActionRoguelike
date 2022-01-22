// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "RoguelikeBTService_CheckHealth.generated.h"

UCLASS()
class ACTIONROGUELIKE_API URoguelikeBTService_CheckHealth : public UBTService
{
	GENERATED_BODY()

public:

	URoguelikeBTService_CheckHealth();

protected:

	// Whether or not the bot should hide and heal
	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector HideAndHealKey;

	// Percent, as decimal, of health to trigger bot to hide and heal
	UPROPERTY(EditAnywhere, Category = "AI")
	float LowHealthPercentage;
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
