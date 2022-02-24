// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikeInteractionComponent.h"

#include "DrawDebugHelpers.h"
#include "RoguelikeGameplayInterface.h"
#include "RoguelikeWorldUserWidget.h"


static TAutoConsoleVariable<bool> CVarDrawDebugInteraction(
	TEXT("rl.DrawDebugInteraction"), false, TEXT("Enables drawing debug lines for interaction component."), ECVF_Cheat);

URoguelikeInteractionComponent::URoguelikeInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	TraceDistance = 600.0f;
	TraceRadius = 30.0f;
	CollisionChannel = ECC_WorldDynamic;
}

void URoguelikeInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void URoguelikeInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                   FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn->IsLocallyControlled())
	{
		FindBestInteractable();
	}
}

void URoguelikeInteractionComponent::FindBestInteractable()
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

	APlayerCameraManager* CameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;

	FVector SweepStart = CameraManager->GetCameraLocation();
	FVector SweepEnd = SweepStart + (CameraManager->GetCameraRotation().Vector() * TraceDistance);

	TArray<FHitResult> Hits;

	FCollisionShape Shape;
	Shape.SetSphere(TraceRadius);

	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, SweepStart, SweepEnd, FQuat::Identity,
	                                                       ObjectQueryParams, Shape);

	FColor DebugColor = bBlockingHit ? FColor::Green : FColor::Red;

	bool bDrawDebug = CVarDrawDebugInteraction.GetValueOnGameThread();

	// Clear reference before attempting to find focus
	FocusedActor = nullptr;

	for (FHitResult Hit : Hits)
	{
		if (bDrawDebug)
		{
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, TraceRadius, 16, DebugColor, false);
		}

		if (AActor* HitActor = Hit.GetActor())
		{
			if (HitActor->Implements<URoguelikeGameplayInterface>())
			{
				FocusedActor = HitActor;

				break;
			}
		}
	}

	if (FocusedActor)
	{
		if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass))
		{
			DefaultWidgetInstance = CreateWidget<URoguelikeWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}

		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttachedActor = FocusedActor;

			if (!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();
			}
		}
	}
	else
	{
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
	}

	if (bDrawDebug)
	{
		DrawDebugLine(GetWorld(), SweepStart, SweepEnd, DebugColor, false, 0.0f, 0, 0.0f);
	}
}

void URoguelikeInteractionComponent::PrimaryInteract()
{
	ServerInteract(FocusedActor);
}

void URoguelikeInteractionComponent::ServerInteract_Implementation(AActor* InFocusActor)
{
	if (InFocusActor == nullptr)
	{
		if (CVarDrawDebugInteraction.GetValueOnGameThread())
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "No Focused Actor to interact with.");
		}
		return;
	}

	APawn* OwnerPawn = Cast<APawn>(GetOwner());

	IRoguelikeGameplayInterface::Execute_Interact(InFocusActor, OwnerPawn);
}
