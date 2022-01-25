// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "GameFramework/GameModeBase.h"
#include "SGameModeBase.generated.h"

class UEnvQuery;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASGameModeBase();
	
	virtual void StartPlay() override;

protected:

	FTimerHandle TimerHandle_SpawnBots;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerInterval;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> MinionClass;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyCurve;

	UPROPERTY(EditDefaultsOnly, Category = "Coins")
	UEnvQuery* SpawnCoinsQuery;
	
	UPROPERTY(EditDefaultsOnly, Category = "Coins")
	int32 DesiredCoinCount;

	UPROPERTY(EditDefaultsOnly, Category = "Coins")
	TSubclassOf<AActor> CoinClass;

	UPROPERTY(EditDefaultsOnly, Category = "Coins")
	float RequiredPowerupSeparation;

	UFUNCTION()
	void SPawnBotTimerElapsed();
	
	UFUNCTION()
	void OnSpawnBotQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);

	UFUNCTION()
	void OnSpawnCoinQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
	
public:
	
	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);

	UFUNCTION(Exec)
	void KillAll();
};