// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguelikeGameModeBase.h"

#include "EngineUtils.h"
#include "RoguelikeActionComponent.h"
#include "RoguelikeAttributeComponent.h"
#include "RoguelikeCharacter.h"
#include "RoguelikeGameplayInterface.h"
#include "RoguelikeMonsterData.h"
#include "RoguelikePlayerState.h"
#include "RoguelikeSaveGame.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "AI/RoguelikeAICharacter.h"
#include "Engine/AssetManager.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"


static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("rl.SpawnBots"), true,
                                                TEXT("Enable spawning of bots via timer."), ECVF_Cheat);

ARoguelikeGameModeBase::ARoguelikeGameModeBase()
{
	BotSpawnTimerInterval = 2.0f;

	CreditAmountKillMinion = 5.0f;

	SlotName = "SaveGame01";
}

void ARoguelikeGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(SpawnBotsTimerHandle, this, &ARoguelikeGameModeBase::SpawnBotTimerElapsed,
	                                BotSpawnTimerInterval, true);

	UEnvQueryInstanceBlueprintWrapper* ItemPickupQueryInstance = UEnvQueryManager::RunEQSQuery(this,
		ItemPickupQuery, this, EEnvQueryRunMode::AllMatching, nullptr);
	if (ensure(ItemPickupQueryInstance))
	{
		ItemPickupQueryInstance->GetOnQueryFinishedEvent().AddDynamic(
			this, &ARoguelikeGameModeBase::OnItemPickupQueryCompleted);
	}
}

void ARoguelikeGameModeBase::OnActorKilled(AActor* VictimActor, AActor* KillerActor)
{
	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim: %s, Killer: %s"), *GetNameSafe(VictimActor),
	       *GetNameSafe(KillerActor));

	ARoguelikeCharacter* Player = Cast<ARoguelikeCharacter>(VictimActor);
	if (Player)
	{
		FTimerHandle RespawnDelayTimerHandle;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());

		float RespawnDelay = 5.f;
		GetWorldTimerManager().SetTimer(RespawnDelayTimerHandle, Delegate, RespawnDelay, false);
	}

	ARoguelikeAICharacter* Minion = Cast<ARoguelikeAICharacter>(VictimActor);
	if (Minion)
	{
		Player = Cast<ARoguelikeCharacter>(KillerActor);
		if (Player)
		{
			ARoguelikePlayerState* PlayerState = Player->GetPlayerState<ARoguelikePlayerState>();
			if (ensure(PlayerState))
			{
				PlayerState->AddCredits(CreditAmountKillMinion);
			}
		}
	}
}

void ARoguelikeGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	LoadSaveGame();
}

void ARoguelikeGameModeBase::WriteSaveGame()
{
	// No player ID system, only save state of first player
	ARoguelikePlayerState* PlayerState = Cast<ARoguelikePlayerState>(GameState->PlayerArray[0]);
	if (PlayerState)
	{
		PlayerState->SavePlayerState(CurrentSaveGame);
	}

	CurrentSaveGame->SavedActors.Empty();

	for (FActorIterator Iterator(GetWorld()); Iterator; ++Iterator)
	{
		AActor* Actor = *Iterator;
		// Only track gameplay actors
		if (!Actor->Implements<URoguelikeGameplayInterface>())
		{
			continue;
		}

		FActorSaveData ActorData;
		ActorData.ActorName = Actor->GetName();
		ActorData.ActorTransform = Actor->GetActorTransform();

		// Save actor properties marked as SaveGame
		FMemoryWriter MemoryWriter(ActorData.CustomActorData);
		FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
		Archive.ArIsSaveGame = true;
		Actor->Serialize(Archive);

		CurrentSaveGame->SavedActors.Add(ActorData);
	}

	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
}

void ARoguelikeGameModeBase::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		CurrentSaveGame = Cast<URoguelikeSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load SaveGame Data."));
			return;
		}

		UE_LOG(LogTemp, Log, TEXT("Loaded SaveGame Data."));

		for (FActorIterator Iterator(GetWorld()); Iterator; ++Iterator)
		{
			AActor* Actor = *Iterator;
			// Only track gameplay actors
			if (!Actor->Implements<URoguelikeGameplayInterface>())
			{
				continue;
			}

			for (FActorSaveData ActorData : CurrentSaveGame->SavedActors)
			{
				if (ActorData.ActorName == Actor->GetName())
				{
					Actor->SetActorTransform(ActorData.ActorTransform);

					// Load actor properties marked as SaveGame
					FMemoryReader MemoryReader(ActorData.CustomActorData);
					FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
					Archive.ArIsSaveGame = true;
					Actor->Serialize(Archive);

					IRoguelikeGameplayInterface::Execute_OnActorLoaded(Actor);

					break;
				}
			}
		}
	}
	else
	{
		CurrentSaveGame = Cast<URoguelikeSaveGame>(
			UGameplayStatics::CreateSaveGameObject(URoguelikeSaveGame::StaticClass()));

		UE_LOG(LogTemp, Log, TEXT("Created New SaveGame Data."));
	}
}

void ARoguelikeGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	ARoguelikePlayerState* PlayerState = NewPlayer->GetPlayerState<ARoguelikePlayerState>();
	if (PlayerState)
	{
		PlayerState->LoadPlayerState(CurrentSaveGame);
	}

	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
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
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ARoguelikeGameModeBase::OnSpawnBotQueryCompleted);
	}
}

void ARoguelikeGameModeBase::OnSpawnBotQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
                                                      EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn Bot EQS query failed!"));
		return;
	}

	TArray<FVector> SpawnLocations;

	if (QueryInstance->GetQueryResultsAsLocations(SpawnLocations))
	{
		if (MonsterTable)
		{
			TArray<FMonsterInfoRow*> MonsterRows;
			MonsterTable->GetAllRows("", MonsterRows);

			int32 RandomIndex = FMath::RandRange(0, MonsterRows.Num() - 1);
			FMonsterInfoRow* SelectedMonsterRow = MonsterRows[RandomIndex];

			UAssetManager* Manager = UAssetManager::GetIfValid();
			if (Manager)
			{
				LogOnScreen(this, "Loading monster...", FColor::Green);
				
				TArray<FName> Bundles;
				FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(
					this, &ARoguelikeGameModeBase::OnMonsterLoaded, SelectedMonsterRow->MonsterId, SpawnLocations[0]);
				Manager->LoadPrimaryAsset(SelectedMonsterRow->MonsterId, Bundles, Delegate);
			}
		}
	}
}

void ARoguelikeGameModeBase::OnMonsterLoaded(FPrimaryAssetId MonsterId, FVector SpawnLocation)
{
	LogOnScreen(this, "Finished loading.", FColor::Green);
	
	UAssetManager* Manager = UAssetManager::GetIfValid();
	if (Manager)
	{
		URoguelikeMonsterData* MonsterData = Cast<URoguelikeMonsterData>(Manager->GetPrimaryAssetObject(MonsterId));
		if (MonsterData)
		{
			AActor* NewBot = GetWorld()->SpawnActor<AActor>(MonsterData->MonsterClass, SpawnLocation,
			                                                FRotator::ZeroRotator);
			if (NewBot)
			{
				LogOnScreen(this, FString::Printf(TEXT("Spawned enemy: %s (%s)"), *GetNameSafe(NewBot),
				                                  *GetNameSafe(MonsterData)));

				URoguelikeActionComponent* BotActionComp = Cast<URoguelikeActionComponent>(
					NewBot->GetComponentByClass(URoguelikeActionComponent::StaticClass()));
				if (BotActionComp)
				{
					for (TSubclassOf<URoguelikeAction> ActionClass : MonsterData->Actions)
					{
						BotActionComp->AddAction(NewBot, ActionClass);
					}
				}
			}
		}
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

void ARoguelikeGameModeBase::OnItemPickupQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
                                                        EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Item Pickup EQS query failed!"));
		return;
	}

	TArray<FVector> SpawnLocations;

	if (QueryInstance->GetQueryResultsAsLocations(SpawnLocations))
	{
		for (FVector SpawnLocation : SpawnLocations)
		{
			// 30% chance to spawn an item at all
			bool bSpawnItem = FMath::RandRange(1, 10) <= 3;
			if (bSpawnItem)
			{
				// 10% chance to spawn a health potion, otherwise spawn a coin
				bool bSpawnHealthPotion = FMath::RandRange(1, 10) == 1;
				if (bSpawnHealthPotion)
				{
					// We want the item to spawn 60 units off the ground and the query is 20 units above the ground
					SpawnLocation.Z += 40;
					GetWorld()->SpawnActor<AActor>(HealthPotionClass, SpawnLocation, FRotator::ZeroRotator);
				}
				else
				{
					// We want the item to spawn 120 units off the ground and the query is 20 units above the ground
					SpawnLocation.Z += 100 - QueryInstance->GetQueryInstance()->GetBoundingBox().GetSize().Z;
					GetWorld()->SpawnActor<AActor>(CoinPickupClass, SpawnLocation, FRotator::ZeroRotator);
				}
			}
		}
	}
}
