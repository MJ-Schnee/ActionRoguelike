// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikeActionComponent.h"
#include "RoguelikeAction.h"

static TAutoConsoleVariable<bool> CVarDebugActionComponent(
	TEXT("rl.DebugActionComponent"), false, TEXT("Show debug messages from the Action Component."), ECVF_Cheat);

URoguelikeActionComponent::URoguelikeActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}

void URoguelikeActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<URoguelikeAction> ActionClass : DefaultActions)
	{
		AddAction(GetOwner(), ActionClass);
	}
}

void URoguelikeActionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CVarDebugActionComponent.GetValueOnGameThread())
	{
		FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);
	}
}

void URoguelikeActionComponent::AddAction(AActor* Instigator, TSubclassOf<URoguelikeAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	URoguelikeAction* NewAction = NewObject<URoguelikeAction>(this, ActionClass);
	if (ensure(NewAction))
	{
		Actions.Add(NewAction);

		if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator)))
		{
			NewAction->StartAction(Instigator);
		}
	}
}

void URoguelikeActionComponent::RemoveAction(URoguelikeAction* ActionToRemove)
{
	if (!ensure(ActionToRemove && !ActionToRemove->IsRunning()))
	{
		return;
	}

	Actions.Remove(ActionToRemove);
}

void URoguelikeActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator, ActionName);
}

bool URoguelikeActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for (URoguelikeAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (!Action->CanStart(Instigator))
			{
				if (CVarDebugActionComponent.GetValueOnGameThread())
				{
					FString FailedMsg = FString::Printf(TEXT("Failed to run: %s"), *ActionName.ToString());
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailedMsg);
				}
				continue;
			}

			// Call server RPC if client
			if (!GetOwner()->HasAuthority())
			{
				ServerStartAction(Instigator, ActionName);	
			}

			Action->StartAction(Instigator);
			return true;
		}
	}

	return false;
}

bool URoguelikeActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (URoguelikeAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (Action->IsRunning())
			{
				Action->StopAction(Instigator);
				return true;
			}
		}
	}

	return false;
}
