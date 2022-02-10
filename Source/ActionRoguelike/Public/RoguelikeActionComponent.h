// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RoguelikeActionComponent.generated.h"

class URoguelikeAction;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONROGUELIKE_API URoguelikeActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URoguelikeActionComponent();

protected:
	UPROPERTY(EditAnywhere, Category = "Actions")
	TArray<TSubclassOf<URoguelikeAction>> DefaultActions;
	
	UPROPERTY()
	TArray<URoguelikeAction*> Actions;

	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable, Category = "Actions")
	void AddAction(TSubclassOf<URoguelikeAction> ActionClass);
	
	UFUNCTION(BlueprintCallable, Category = "Action")
	bool StartActionByName(AActor* Instigator, FName ActionName);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool StopActionByName(AActor* Instigator, FName ActionName);
};
