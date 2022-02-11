// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikeAction.h"

#include "RoguelikeActionComponent.h"

bool URoguelikeAction::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning())
	{
		return false;
	}
	
	URoguelikeActionComponent* OwningComp = GetOwningComponent();

	if (OwningComp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}

	return true;
}

void URoguelikeAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Action \"%s\" has been started"), *GetNameSafe(this));

	URoguelikeActionComponent* OwningComp = GetOwningComponent();
	OwningComp->ActiveGameplayTags.AppendTags(GrantsTags);

	bIsRunning = true;
}

void URoguelikeAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Action \"%s\" has been stopped"), *GetNameSafe(this));

	ensureAlways(bIsRunning);

	URoguelikeActionComponent* OwningComp = GetOwningComponent();
	OwningComp->ActiveGameplayTags.RemoveTags(GrantsTags);

	bIsRunning = false;
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

URoguelikeActionComponent* URoguelikeAction::GetOwningComponent() const
{
	return Cast<URoguelikeActionComponent>(GetOuter());
}

bool URoguelikeAction::IsRunning() const
{
	return bIsRunning;
}
