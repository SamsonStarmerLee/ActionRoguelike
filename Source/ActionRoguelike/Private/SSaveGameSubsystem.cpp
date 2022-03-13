// Fill out your copyright notice in the Description page of Project Settings.

#include "SSaveGameSubsystem.h"
#include "EngineUtils.h"
#include "SGameplayInterface.h"
#include "SPlayerState.h"
#include "SSaveGameSettings.h"
#include "Engine/DataTable.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

void USSaveGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const USSaveGameSettings* Settings = GetDefault<USSaveGameSettings>();
	CurrentSlotName = Settings->SaveSlotName; // Access defaults from DefaultGame.ini

	// Make sure it's loaded into memory .Get() only resolves if already loaded previously elsewhere in code.
	// We don't need this table for anything, just a reference example.
	UDataTable* DummyTable = Settings->DummyTablePath.LoadSynchronous();
}

void USSaveGameSubsystem::HandleStartingNewPlayer(const APlayerController* NewPlayer) const
{
	ASPlayerState* PlayerState = NewPlayer->GetPlayerState<ASPlayerState>();
	if (ensure(PlayerState))
	{
		PlayerState->LoadPlayerState(CurrentSaveGame);
	}
}

bool USSaveGameSubsystem::OverrideSpawnTransform(APlayerController* NewPlayer)
{
	if (NewPlayer == nullptr || NewPlayer->IsPendingKill())
	{
		return false;
	}

	ASPlayerState* PlayerState = NewPlayer->GetPlayerState<ASPlayerState>();
	if (PlayerState)
	{
		return false;
	}

	if (APawn* Pawn = PlayerState->GetPawn())
	{
		FPlayerSaveData* FoundData = CurrentSaveGame->GetPlayerData(PlayerState);
		if (FoundData && FoundData->bResumeAtTransform)
		{
			Pawn->SetActorLocation(FoundData->Location);
			Pawn->SetActorRotation(FoundData->Rotation);

			// PlayerState owner is a (Player)Controller.
			AController* Controller = Cast<AController>(PlayerState->GetOwner());
			Controller->SetControlRotation(FoundData->Rotation);
			return true;
		}
	}

	return false;
}

void USSaveGameSubsystem::SetSlotName(FString NewSlotName)
{
	// Ignore Empty Name
	if (NewSlotName.Len() == 0)
	{
		return;
	}

	CurrentSlotName = NewSlotName;
}

void USSaveGameSubsystem::WriteSaveGame()
{
	CurrentSaveGame->SavedPlayers.Empty();
	CurrentSaveGame->SavedActors.Empty();
	
	AGameStateBase* GameStateBase = GetWorld()->GetGameState();
	if (GameStateBase == nullptr)
	{
		// Warn about failure to save?
		return;
	}
	
	// Iterate all player states, we don't have proper ID to match yet (requires Steam or EOS)
	for (int32 i = 0; i < GameStateBase->PlayerArray.Num(); i++)
	{
		ASPlayerState* PS = Cast<ASPlayerState>(GameStateBase->PlayerArray[i]);
		if (PS)
		{
			PS->SavePlayerState(CurrentSaveGame);
			break; // single player only at this point
		}
	}

	// Iterate the entire world of actors
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		// Only interested in our 'gameplay actors', skip actors that are being destroyed
		// Note: You might instead use a dedicated SavableObject interface for Actors you want to save instead of re-using GameplayInterface
		if (Actor->IsPendingKill() || !Actor->Implements<USGameplayInterface>())
		{
			continue;
		}

		FActorSaveData ActorData;
		ActorData.ActorName = Actor->GetFName();
		ActorData.Transform = Actor->GetActorTransform();
		
		// Pass the array to fill with data from Actor
		FMemoryWriter MemWriter(ActorData.ByteData);

		FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
		// Find only variables with UPROPERTY(SaveGame)
		Ar.ArIsSaveGame = true;
		// Converts Actor's SaveGame UPROPERTIES into binary array
		Actor->Serialize(Ar);

		CurrentSaveGame->SavedActors.Add(ActorData);
	}

	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, CurrentSlotName, 0);

	OnSaveGameWritten.Broadcast(CurrentSaveGame);
}

void USSaveGameSubsystem::LoadSaveGame(FString InSlotName)
{
	// Update slot name first if specified, otherwise keeps default name
	SetSlotName(InSlotName);
	
	if (UGameplayStatics::DoesSaveGameExist(CurrentSlotName, 0))
	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::LoadGameFromSlot(CurrentSlotName, 0));
		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load SaveGame Data."));
			return;
		}

		UE_LOG(LogTemp, Log, TEXT("Loaded SaveGame Data."));


		// Iterate the entire world of actors
		for (FActorIterator It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;
			// Only interested in our 'gameplay actors'
			if (!Actor->Implements<USGameplayInterface>())
			{
				continue;
			}

			for (FActorSaveData ActorData : CurrentSaveGame->SavedActors)
			{
				if (ActorData.ActorName == Actor->GetFName())
				{
					Actor->SetActorTransform(ActorData.Transform);

					FMemoryReader MemReader(ActorData.ByteData);

					FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
					Ar.ArIsSaveGame = true;
					// Convert binary array back into actor's variables
					Actor->Serialize(Ar);

					ISGameplayInterface::Execute_OnActorLoaded(Actor);

					break;
				}
			}
		}

		OnSaveGameLoaded.Broadcast(CurrentSaveGame);
	}
	else
	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::CreateSaveGameObject(USSaveGame::StaticClass()));

		UE_LOG(LogTemp, Log, TEXT("Created New SaveGame Data."));
	}
}