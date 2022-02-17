// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/RoguelikeBTService_CheckAtkRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void URoguelikeBTService_CheckAtkRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                                                 float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackboardComp))
	{
		AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor"));
		if (TargetActor)
		{
			AAIController* Controller = OwnerComp.GetAIOwner();
			if (ensure(Controller))
			{
				APawn* AIPawn = Controller->GetPawn();
				if (ensure(AIPawn))
				{
					float DistanceToTarget = FVector::Distance(TargetActor->GetActorLocation(),
					                                           AIPawn->GetActorLocation());

					bool bWithinRange = DistanceToTarget < 3000.f;

					bool bTargetActorVisible = false;
					if (bWithinRange)
					{
						bTargetActorVisible = Controller->LineOfSightTo(TargetActor);
					}

					BlackboardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName,
					                               (bWithinRange && bTargetActorVisible));

					return;
				}
			}
		}

		BlackboardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, false);
	}
}
