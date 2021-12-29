// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikeInteractionComponent.h"

#include "DrawDebugHelpers.h"
#include "RoguelikeGameplayInterface.h"

// Sets default values for this component's properties
URoguelikeInteractionComponent::URoguelikeInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URoguelikeInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void URoguelikeInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void URoguelikeInteractionComponent::PrimaryInteract()
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	AActor* Owner = GetOwner();
	
	FVector EyeLocation;
	FRotator EyeRotation;
	Owner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	
	FVector End = EyeLocation + (EyeRotation.Vector() * 350);

	TArray<FHitResult> Hits;

	FCollisionShape Shape;
	float ShapeRadius = 30.0f;
	Shape.SetSphere(ShapeRadius);

	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity,
		ObjectQueryParams, Shape);

	FColor DebugColor = bBlockingHit ? FColor::Green : FColor::Red;
	
	for (FHitResult Hit : Hits)
	{
		if (AActor* HitActor = Hit.GetActor())
		{
			if (HitActor->Implements<URoguelikeGameplayInterface>())
			{
				APawn* OwnerPawn = Cast<APawn>(Owner);
			
				IRoguelikeGameplayInterface::Execute_Interact(HitActor, OwnerPawn);
				break;
			}
		}
		
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, ShapeRadius, 16, DebugColor, false, 2.0f);
	}

	DrawDebugLine(GetWorld(), EyeLocation, End, DebugColor, false, 2.0f, 0, 2.0f);
}

