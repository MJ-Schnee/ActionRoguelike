// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikeAction_ProjectileAttack.h"

#include "DrawDebugHelpers.h"
#include "RoguelikeCharacter.h"
#include "RoguelikeProjectile.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

static TAutoConsoleVariable<bool> CVarDrawDebugAttack(TEXT("rl.DrawDebugAttack"), false,
	TEXT("Enables drawing debug lines for interaction component."), ECVF_Cheat);

URoguelikeAction_ProjectileAttack::URoguelikeAction_ProjectileAttack()
{
	HandSocketName = "Muzzle_01";
	AttackAnimDelay = 0.2f;
}

void URoguelikeAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	ACharacter* InstigatorCharacter = Cast<ACharacter>(Instigator);
	if (InstigatorCharacter)
	{
		InstigatorCharacter->PlayAnimMontage(AttackAnim);

		UGameplayStatics::SpawnEmitterAttached(CastingEffect, InstigatorCharacter->GetMesh(), HandSocketName);

		FTimerHandle TimerHandle_AttackDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "AttackDelay_Elapsed", InstigatorCharacter);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackAnimDelay, false);
	}
}

void URoguelikeAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{
	if (ensureAlways(ProjectileClass))
	{
		FVector TraceStart = InstigatorCharacter->GetPawnViewLocation();
		FVector TraceEnd = TraceStart + InstigatorCharacter->GetControlRotation().Vector() * 5000.0f;

		FCollisionShape Shape;
		float ShapeRadius = 20.0f;
		Shape.SetSphere(ShapeRadius);
	
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(InstigatorCharacter);

		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);
	
		bool bDrawDebugAttack = CVarDrawDebugAttack.GetValueOnGameThread();
		FColor DebugColor = FColor::Green;
		TArray<FHitResult> Hits;
		if (GetWorld()->SweepMultiByObjectType(Hits, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params))
		{
			for (FHitResult Hit : Hits)
			{
				if (bDrawDebugAttack)
				{
					DrawDebugLine(GetWorld(), TraceStart, Hit.ImpactPoint, DebugColor, false, 2.0f, 0, 2.0f);
					DrawDebugSphere(GetWorld(), Hit.Location, ShapeRadius, 16, DebugColor, false, 2.0f);	
				}
			
				// Dismiss player spawned projectiles
				ARoguelikeProjectile* Projectile = Cast<ARoguelikeProjectile>(Hit.Actor);
				bool bPlayerProjectile = Projectile != nullptr &&
										 Cast<ARoguelikeCharacter>(Projectile->GetInstigator()) != nullptr;
				if (!bPlayerProjectile)
				{
					// Override TraceEnd for determining impact point
					TraceEnd = Hit.ImpactPoint;
					break;
				}
			}
		}

		FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocketName);

		FRotator RotationToCrosshair = (TraceEnd - HandLocation).Rotation();
	
		FTransform SpawnTransform = FTransform(RotationToCrosshair, HandLocation);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = InstigatorCharacter;
	
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTransform, SpawnParams);
	}

	StopAction(InstigatorCharacter);
}
