// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RoguelikeBTTask_RangedAttack.generated.h"

UCLASS()
class ACTIONROGUELIKE_API URoguelikeBTTask_RangedAttack : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:

	URoguelikeBTTask_RangedAttack();

protected:

	UPROPERTY(EditAnywhere, Category = "AI")	
	float MaxBulletSpread;

	UPROPERTY(EditAnywhere, Category = "AI")
	TSubclassOf<AActor> ProjectileClass;
};
