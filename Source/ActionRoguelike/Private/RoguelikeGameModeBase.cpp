// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikeGameModeBase.h"

#include "EngineUtils.h"
#include "RoguelikeAttributeComponent.h"
#include "RoguelikeCharacter.h"
#include "AI/RoguelikeAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"


static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("rl.SpawnBots"), true,
	TEXT("Enable spawning of bots via timer."), ECVF_Cheat);

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

void ARoguelikeGameModeBase::OnActorKilled(AActor* VictimActor, AActor* KillerActor)
{
	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim: %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(KillerActor));
	
	ARoguelikeCharacter* Player = Cast<ARoguelikeCharacter>(VictimActor);
	if (Player)
	{
		FTimerHandle RespawnDelayTimerHandle;
		
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());

		float RespawnDelay = 5.f;
		GetWorldTimerManager().SetTimer(RespawnDelayTimerHandle, Delegate, RespawnDelay, false);
	}
}

void ARoguelikeGameModeBase::SpawnBotTimerElapsed()
{
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bot spawning has been disabled via cvar 'CVarSpawnBots'"));
		
		return;
	}
	
	int32 NumBotsAlive = 0;
	for (TActorIterator<ARoguelikeAICharacter> Iterator(GetWorld()); Iterator; ++Iterator)
	{
		ARoguelikeAICharacter* Bot = *Iterator;

		URoguelikeAttributeComponent* AttributeComp = URoguelikeAttributeComponent::GetAttributes(Bot);
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			NumBotsAlive++;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Num alive bots: %i"), NumBotsAlive);

	float MaxBotCount = 10.0f;
	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->GetTimeSeconds());
	}
	
	if (NumBotsAlive >= MaxBotCount)
	{
		UE_LOG(LogTemp, Warning, TEXT("Bots at maximum capacity. Skipping bot spawn"));
		return;
	}
	
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
	
	TArray<FVector> SpawnLocations = QueryInstance->GetResultsAsLocations();

	if (SpawnLocations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, SpawnLocations[0], FRotator::ZeroRotator);
	}
}

void ARoguelikeGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();
		RestartPlayer(Controller);
	}
}
