// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RoguelikeAction.generated.h"

class URoguelikeActionComponent;
UCLASS(Blueprintable)
class ACTIONROGUELIKE_API URoguelikeAction : public UObject
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable, Category = "Action")
	URoguelikeActionComponent* GetOwningComponent() const;

	// Tags added to owning actor when activated and removed when action stops
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	// Action can't start if any of these tags are active
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	bool bIsRunning;

public:
	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning() const;

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);

	// Action nickname to start/stop without referencing object
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	virtual UWorld* GetWorld() const override;
};
