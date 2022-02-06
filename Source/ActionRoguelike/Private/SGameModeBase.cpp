// Fill out your copyright notice in the Description page of Project Settings.

#include "SGameModeBase.h"

#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "SGameplayInterface.h"
#include "SPlayerState.h"
#include "AI/SAICharacter.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/SaveGame.h"
#include "Kismet/GameplayStatics.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), true, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);

ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval = 2.0f;
}

void ASGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	LoadSaveGame();
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	// Continuous timer to spawn in more bots.
	// Actual amount of bots and whether its allowed to spawn determined by spawn logic later in the chain...
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SPawnBotTimerElapsed, SpawnTimerInterval, true);

	// Spawn random coins around map
	const auto QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnCoinsQuery, this, EEnvQueryRunMode::AllMatching, nullptr);
	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnSpawnCoinQueryCompleted);
	}
}

void ASGameModeBase::SPawnBotTimerElapsed()
{
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bot Spawning disabled via cvar 'CVarSpawnBots'."));
		return;
	}
	
	int32 NumAliveBots = 0;
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		if (USAttributeComponent::IsActorAlive(*It))
		{
			NumAliveBots++;
		}
	}

	float MaxBotCount = 10.f;

	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	if (NumAliveBots >= MaxBotCount)
	{
		UE_LOG(LogTemp, Log, TEXT("At maximum bot capacity. Skipping spawn."));
		return;
	}
	
	const auto QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnSpawnBotQueryCompleted);
	}
}

void ASGameModeBase::OnSpawnBotQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, const EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed!"));
		return;
	}
	
	TArray<FVector> Locations;
	if (QueryInstance->GetQueryResultsAsLocations(Locations) && Locations.Num() > 0)
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
	}
}

void ASGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();
		RestartPlayer(Controller);
	}
}

void ASGameModeBase::OnSpawnCoinQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn coins EQS Query Failed!"));
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	TArray<FVector> UsedLocations;
	int32 SpawnCounter = 0;
	
	while (SpawnCounter < DesiredCoinCount && Locations.Num() - 1)
	{
		// Pick a random location from remaining points
		const int32 RandomIndex = FMath::RandRange(0, Locations.Num() - 1);
		const FVector SelectedLocation = Locations[RandomIndex];
		Locations.RemoveAt(RandomIndex);

		// Check minimum distance requirement
		bool bValidLocation = true;
		for (FVector OtherLocation : UsedLocations)
		{
			float DistanceTo = (SelectedLocation - OtherLocation).Size();

			if (DistanceTo < RequiredPowerupSeparation)
			{
				// Show Skipped locations due to distance
				// DrawDebugSphere(GetWorld(), RandomLocation, 50.0f, 20, FColor::Red, false, 10.f);

				bValidLocation = false;
				break;
			}
		}

		if (!bValidLocation)
		{
			continue;
		}

		GetWorld()->SpawnActor<AActor>(CoinClass, SelectedLocation, FRotator::ZeroRotator);
		UsedLocations.Add(SelectedLocation);
		SpawnCounter++;
	}
}

void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	// Player death and respawn
	const auto PlayerCharacter = Cast<ASCharacter>(VictimActor);
	if (PlayerCharacter)
	{
		FTimerHandle TimerHandle_RespawnDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElapsed", PlayerCharacter->GetController());

		constexpr float RespawnDelay = 2.0f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);
	}

	// AI Death and Credit Reward
	const auto AICharacter = Cast<ASAICharacter>(VictimActor);
	if (AICharacter)
	{
		const auto PlayerState = AICharacter->GetPlayerState<ASPlayerState>();
		if (PlayerState)
		{
			PlayerState->AddCredits(1);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim %s, Killer %s"), *GetNameSafe(VictimActor), *GetNameSafe(Killer));
}

void ASGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	ASPlayerState* State = NewPlayer->GetPlayerState<ASPlayerState>();
	if (State)
	{
		State->LoadPlayerState(CurrentSaveGame);
	}
}

void ASGameModeBase::KillAll()
{
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		const auto Bot = *It;
		const auto AttributeComponent = USAttributeComponent::GetAttributes(Bot);

		if (ensure(AttributeComponent) && USAttributeComponent::IsActorAlive(Bot))
		{
			AttributeComponent->Kill(this); // TODO: Pass in player? for kill credit
		}
	}
}

void ASGameModeBase::WriteSaveGame()
{
	for (int32 i = 0; i < GameState->PlayerArray.Num(); i++)
	{
		ASPlayerState* State = Cast<ASPlayerState>(GameState->PlayerArray[i]);
		if (State)
		{
			State->SavePlayerState(CurrentSaveGame);
		}
	}

	CurrentSaveGame->SavedActors.Empty();
	
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		const AActor* Actor = *It;
		if (!Actor->Implements<USGameplayInterface>())
		{
			continue;
		}

		FActorSaveData ActorData;
		ActorData.ActorName = Actor->GetName();
		ActorData.Transform = Actor->GetTransform();

		CurrentSaveGame->SavedActors.Add(ActorData);
	}
	
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
}

void ASGameModeBase::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
	  	CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load SaveGame data."));
			return;
		}

		UE_LOG(LogTemp, Log, TEXT("Loaded SaveGame data."));

		for (FActorIterator It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;
			if (!Actor->Implements<USGameplayInterface>())
			{
				continue;
			}

			for (FActorSaveData ActorData : CurrentSaveGame->SavedActors)
			{
				if (ActorData.ActorName == Actor->GetName())
				{
					Actor->SetActorTransform(ActorData.Transform);
				}
			}
		}
	}
	else
	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::CreateSaveGameObject(USSaveGame::StaticClass()));
		UE_LOG(LogTemp, Log, TEXT("Created new SaveGame data."));
	}
}