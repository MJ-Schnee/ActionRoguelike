// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikeInteractionComponent.h"

#include "DrawDebugHelpers.h"
#include "RoguelikeGameplayInterface.h"

URoguelikeInteractionComponent::URoguelikeInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void URoguelikeInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void URoguelikeInteractionComponent::PrimaryInteract()
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	APlayerCameraManager* CameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;

	FVector SweepStart = CameraManager->GetCameraLocation();
	FVector SweepEnd = SweepStart + (CameraManager->GetCameraRotation().Vector() * 600.0f);

	TArray<FHitResult> Hits;

	FCollisionShape Shape;
	float ShapeRadius = 30.0f;
	Shape.SetSphere(ShapeRadius);

	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, SweepStart, SweepEnd, FQuat::Identity,
		ObjectQueryParams, Shape);

	FColor DebugColor = bBlockingHit ? FColor::Green : FColor::Red;
	
	for (FHitResult Hit : Hits)
	{
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, ShapeRadius, 16, DebugColor, false, 2.0f);
		
		if (AActor* HitActor = Hit.GetActor())
		{
			if (HitActor->Implements<URoguelikeGameplayInterface>())
			{
				APawn* OwnerPawn = Cast<APawn>(GetOwner());
			
				IRoguelikeGameplayInterface::Execute_Interact(HitActor, OwnerPawn);
				break;
			}
		}
	}

	DrawDebugLine(GetWorld(), SweepStart, SweepEnd, DebugColor, false, 2.0f, 0, 2.0f);
}
