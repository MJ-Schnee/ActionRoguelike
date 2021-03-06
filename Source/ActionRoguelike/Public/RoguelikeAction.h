// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RoguelikeAction.generated.h"

class URoguelikeActionComponent;

USTRUCT()
struct FActionRepData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	bool bIsRunning;

	UPROPERTY()
	AActor* Instigator;
};

UCLASS(Blueprintable)
class ACTIONROGUELIKE_API URoguelikeAction : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(Replicated)
	URoguelikeActionComponent* ActionComp;

	UFUNCTION(BlueprintCallable, Category = "Actions")
	URoguelikeActionComponent* GetOwningComponent() const;

	// Tags added to owning actor when activated and removed when action stops
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	// Action can't start if any of these tags are active
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	UPROPERTY(ReplicatedUsing="OnRep_RepData")
	FActionRepData RepData;

	UPROPERTY(Replicated)
	float TimeStarted;

	UFUNCTION()
	void OnRep_RepData();

public:
	void Initialize(URoguelikeActionComponent* NewActionComp);

	UPROPERTY(EditDefaultsOnly, Category = "Actions")
	bool bAutoStart;

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool IsRunning() const;

	UFUNCTION(BlueprintNativeEvent, Category = "Actions")
	bool CanStart(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Actions")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Actions")
	void StopAction(AActor* Instigator);

	// Action nickname to start/stop without referencing object
	UPROPERTY(EditDefaultsOnly, Category = "Actions")
	FName ActionName;

	virtual UWorld* GetWorld() const override;

	bool IsSupportedForNetworking() const override
	{
		return true;
	}
};
