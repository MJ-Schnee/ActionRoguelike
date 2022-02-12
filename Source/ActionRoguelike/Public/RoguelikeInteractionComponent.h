// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RoguelikeInteractionComponent.generated.h"


class URoguelikeWorldUserWidget;
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONROGUELIKE_API URoguelikeInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Executes Interact() on FocusedActor
	void PrimaryInteract();

protected:
	void FindBestInteractable();
	
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	AActor* FocusedActor;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	TEnumAsByte<ECollisionChannel> CollisionChannel;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<URoguelikeWorldUserWidget> DefaultWidgetClass;

	UPROPERTY()
	URoguelikeWorldUserWidget* DefaultWidgetInstance;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	// Sets default values for this component's properties
	URoguelikeInteractionComponent();
};
