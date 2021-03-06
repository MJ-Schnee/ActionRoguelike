// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "RoguelikeGameModeBase.generated.h"

class URoguelikeSaveGame;
class UEnvQueryInstanceBlueprintWrapper;
class UEnvQuery;
class URoguelikeMonsterData;

USTRUCT(BlueprintType)
struct FMonsterInfoRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	FMonsterInfoRow()
	{
		Weight = 1.0f;
		SpawnCost = 5.0f;
		KillReward = 20.0f;
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FPrimaryAssetId MonsterId;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Weight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float SpawnCost;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float KillReward;
};

UCLASS()
class ACTIONROGUELIKE_API ARoguelikeGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARoguelikeGameModeBase();

	virtual void StartPlay() override;

	virtual void OnActorKilled(AActor* VictimActor, AActor* KillerActor);

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void WriteSaveGame();

	void LoadSaveGame();

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

protected:
	UPROPERTY()
	URoguelikeSaveGame* CurrentSaveGame;

	FString SlotName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UDataTable* MonsterTable;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyCurve;

	FTimerHandle SpawnBotsTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float BotSpawnTimerInterval;

	UPROPERTY(EditDefaultsOnly, Category = "Credits")
	float CreditAmountKillMinion;

	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	UEnvQuery* ItemPickupQuery;

	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	TSubclassOf<AActor> CoinPickupClass;

	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	TSubclassOf<AActor> HealthPotionClass;

	UFUNCTION()
	void SpawnBotTimerElapsed();

	UFUNCTION()
	void OnSpawnBotQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void OnMonsterLoaded(FPrimaryAssetId MonsterId, FVector SpawnLocation);

	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);

	UFUNCTION()
	void OnItemPickupQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
	                                EEnvQueryStatus::Type QueryStatus);
};
