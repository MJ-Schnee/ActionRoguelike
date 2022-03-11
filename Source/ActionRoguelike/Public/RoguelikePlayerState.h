// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RoguelikePlayerState.generated.h"

class ARoguelikePlayerState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, ARoguelikePlayerState*, PlayerState, int32,
                                               NewCredits, int32, Delta);

UCLASS()
class ACTIONROGUELIKE_API ARoguelikePlayerState : public APlayerState
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Replicated, Category = "Credits")
	int32 Credits;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastCreditsChanged(ARoguelikePlayerState* PlayerState, int32 NewCredits, int32 Delta);

public:
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCreditsChanged OnCreditsChanged;

	UFUNCTION(BlueprintCallable, Category = "Credits")
	void AddCredits(int32 Delta);

	UFUNCTION(BlueprintCallable, Category = "Credits")
	bool RemoveCredits(int32 Delta);

	UFUNCTION(BlueprintCallable)
	int32 GetCredits() const;
};
