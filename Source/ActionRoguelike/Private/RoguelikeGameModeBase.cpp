// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikeGameModeBase.h"

#include "EngineUtils.h"
#include "RoguelikeAttributeComponent.h"
#include "AI/RoguelikeAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"


ARoguelikeGameModeBase::ARoguelikeGameModeBase()
{
	SpawnTimerInterval = 2.0f;
}

void ARoguelikeGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(SpawnBotsTimerHandle, this, &ARoguelikeGameModeBase::SpawnBotTimerElapsed,
		SpawnTimerInterval, true);
}

void ARoguelikeGameModeBase::SpawnBotTimerElapsed()
{
	 UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this,
	 	SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ARoguelikeGameModeBase::OnQueryCompleted);
	}
}

void ARoguelikeGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
	EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn Bot EQS query failed!"));
		return;	
	}

	int32 NumBotsAlive = 0;
	for (TActorIterator<ARoguelikeAICharacter> Iterator(GetWorld()); Iterator; ++Iterator)
	{
		ARoguelikeAICharacter* Bot = *Iterator;

		URoguelikeAttributeComponent* AttributeComp = Cast<URoguelikeAttributeComponent>(
			Bot->GetComponentByClass(URoguelikeAttributeComponent::StaticClass()));

		if (AttributeComp && AttributeComp->IsAlive())
		{
			NumBotsAlive++;
		}
	}

	float MaxBotCount = 10.0f;
	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->GetTimeSeconds());
	}
	
	if (NumBotsAlive >= MaxBotCount)
	{
		return;
	}
	
	TArray<FVector> SpawnLocations = QueryInstance->GetResultsAsLocations();

	if (SpawnLocations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, SpawnLocations[0], FRotator::ZeroRotator);
	}
}