// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikeActionEffect.h"

#include "RoguelikeActionComponent.h"
#include "GameFramework/GameStateBase.h"

URoguelikeActionEffect::URoguelikeActionEffect()
{
	bAutoStart = true;
}

void URoguelikeActionEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	if (Duration > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "StopAction", Instigator);

		GetWorld()->GetTimerManager().SetTimer(DurationHandle, Delegate, Duration, false);
	}

	if (Period > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ExecutePeriodicEffect", Instigator);

		GetWorld()->GetTimerManager().SetTimer(PeriodHandle, Delegate, Period, true);
	}
}

void URoguelikeActionEffect::StopAction_Implementation(AActor* Instigator)
{
	if (GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(Instigator);
	}

	Super::StopAction_Implementation(Instigator);

	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);
	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);

	if (ActionComp != nullptr)
	{
		ActionComp->RemoveAction(this);
	}
}

float URoguelikeActionEffect::TimeRemaining() const
{
	AGameStateBase* GameState = GetWorld()->GetGameState<AGameStateBase>();
	if (GameState)
	{
		float EndTime = TimeStarted + Duration;
		return EndTime - GameState->GetServerWorldTimeSeconds();
	}

	return Duration;
}

void URoguelikeActionEffect::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{
}
