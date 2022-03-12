// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikeAction.h"

#include "RoguelikeActionComponent.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "Net/UnrealNetwork.h"

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
	LogOnScreen(this, FString::Printf(TEXT("Started  %s"), *ActionName.ToString()), FColor::Green);

	URoguelikeActionComponent* OwningComp = GetOwningComponent();
	OwningComp->ActiveGameplayTags.AppendTags(GrantsTags);

	bIsRunning = true;
}

void URoguelikeAction::StopAction_Implementation(AActor* Instigator)
{
	LogOnScreen(this, FString::Printf(TEXT("Stopped %s"), *ActionName.ToString()), FColor::White);

	URoguelikeActionComponent* OwningComp = GetOwningComponent();
	OwningComp->ActiveGameplayTags.RemoveTags(GrantsTags);

	bIsRunning = false;
}

UWorld* URoguelikeAction::GetWorld() const
{
	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor)
	{
		return Actor->GetWorld();
	}

	return nullptr;
}

URoguelikeActionComponent* URoguelikeAction::GetOwningComponent() const
{
	return ActionComp;
}

void URoguelikeAction::OnRep_IsRunning()
{
	if (bIsRunning)
	{
		StartAction(nullptr);
	}
	else
	{
		StopAction(nullptr);
	}
}

void URoguelikeAction::Initialize(URoguelikeActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;
}

bool URoguelikeAction::IsRunning() const
{
	return bIsRunning;
}

void URoguelikeAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URoguelikeAction, bIsRunning);
	DOREPLIFETIME(URoguelikeAction, ActionComp);
}
