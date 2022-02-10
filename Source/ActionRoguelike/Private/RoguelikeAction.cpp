// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikeAction.h"

void URoguelikeAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Action \"%s\" has been started"), *GetNameSafe(this));
}

void URoguelikeAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Action \"%s\" has been stopped"), *GetNameSafe(this));
}

UWorld* URoguelikeAction::GetWorld() const
{
	UActorComponent* Comp = Cast<UActorComponent>(GetOuter());
	if (Comp)
	{
		return Comp->GetWorld();
	}

	return nullptr;
}
