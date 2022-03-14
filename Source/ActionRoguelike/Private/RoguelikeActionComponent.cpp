// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikeActionComponent.h"
#include "RoguelikeAction.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

static TAutoConsoleVariable<bool> CVarDebugActionComponent(
	TEXT("rl.DebugActionComponent"), false, TEXT("Show debug messages from the Action Component."), ECVF_Cheat);
static TAutoConsoleVariable<bool> CVarDebugNetworkActionComponent(
	TEXT("rl.DebugNetworkActionComponent"), false, TEXT("Show NETWORK debug messages from the Action Component."), ECVF_Cheat);

URoguelikeActionComponent::URoguelikeActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}

bool URoguelikeActionComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch,
                                                    FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (URoguelikeAction* Action : Actions)
	{
		if (Action)
		{
			WroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

void URoguelikeActionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->HasAuthority())
	{
		for (TSubclassOf<URoguelikeAction> ActionClass : DefaultActions)
		{
			AddAction(GetOwner(), ActionClass);
		}
	}
}

void URoguelikeActionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Draw All Actions
	if (CVarDebugNetworkActionComponent.GetValueOnGameThread())
	{
		for (URoguelikeAction* Action : Actions)
		{
			FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;

			FString ActionMsg = FString::Printf(TEXT("[%s] Action: %s"), *GetNameSafe(GetOwner()), *GetNameSafe(Action));

			// LogOnScreen(this, ActionMsg, TextColor, 0.0f);
		}
	}
}

void URoguelikeActionComponent::AddAction(AActor* Instigator, TSubclassOf<URoguelikeAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	// Server will add actions to clients
	if (!GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Client attempting to AddAction. [Class: %s]"), *GetNameSafe(ActionClass));
		return;
	}

	URoguelikeAction* NewAction = NewObject<URoguelikeAction>(GetOwner(), ActionClass);
	if (ensure(NewAction))
	{
		NewAction->Initialize(this);

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

void URoguelikeActionComponent::ServerStopAction_Implementation(AActor* Instigator, FName ActionName)
{
	StopActionByName(Instigator, ActionName);
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
			
			Action->StartAction(Instigator);

			if (!GetOwner()->HasAuthority())
			{
				ServerStartAction(Instigator, ActionName);
			}

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
				
				if (!GetOwner()->HasAuthority())
				{
					ServerStopAction(Instigator, ActionName);
				}
				
				return true;
			}
		}
	}

	return false;
}

void URoguelikeActionComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URoguelikeActionComponent, Actions);
}
