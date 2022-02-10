// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikeActionComponent.h"
#include "RoguelikeAction.h"

URoguelikeActionComponent::URoguelikeActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URoguelikeActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<URoguelikeAction> ActionClass : DefaultActions)
	{
		AddAction(ActionClass);
	}
}

void URoguelikeActionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void URoguelikeActionComponent::AddAction(TSubclassOf<URoguelikeAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

 	URoguelikeAction* NewAction = NewObject<URoguelikeAction>(this, ActionClass);
	if (ensure(NewAction))
	{
		Actions.Add(NewAction);
	}
}

bool URoguelikeActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for (URoguelikeAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
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
			Action->StopAction(Instigator);
			return true;
		}
	}

	return false;
}
