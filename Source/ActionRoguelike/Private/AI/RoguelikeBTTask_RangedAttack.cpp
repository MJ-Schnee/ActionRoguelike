// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/RoguelikeBTTask_RangedAttack.h"

#include "AIController.h"
#include "RoguelikeAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

URoguelikeBTTask_RangedAttack::URoguelikeBTTask_RangedAttack()
{
	MaxBulletSpread = 5.0f;
}

EBTNodeResult::Type URoguelikeBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (ensure(Controller))
	{
		ACharacter* AICharacter = Cast<ACharacter>(Controller->GetPawn());
		if (AICharacter == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		FVector MuzzleLocation = AICharacter->GetMesh()->GetSocketLocation("Muzzle_01");
		
		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
		if (TargetActor == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		if (!URoguelikeAttributeComponent::IsActorAlive(TargetActor))
		{
			return EBTNodeResult::Failed;
		}

		FVector AimDirection = TargetActor->GetActorLocation() - MuzzleLocation;
		FRotator MuzzleRotation = AimDirection.Rotation();

		// Random bullet spread
		MuzzleRotation.Pitch += FMath::RandRange(0.0f, MaxBulletSpread);
		MuzzleRotation.Yaw += FMath::RandRange(-MaxBulletSpread, MaxBulletSpread);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = AICharacter;

		AActor* Projectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);

		return Projectile ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}
	
	return EBTNodeResult::Failed;
}
