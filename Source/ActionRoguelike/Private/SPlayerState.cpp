// Fill out your copyright notice in the Description page of Project Settings.

#include "SPlayerState.h"

#include "SSaveGame.h"
#include "Net/UnrealNetwork.h"


int32 ASPlayerState::GetCredits() const
{
	return Credits;
}

void ASPlayerState::AddCredits(const int32 Amount)
{
	if (Amount <= 0)
	{
		return;
	}
	
	Credits += Amount;

	OnCreditsChanged.Broadcast(this, Credits, Amount);
}

bool ASPlayerState::RemoveCredits(const int32 Amount)
{
	if (!ensure(Amount > 0))
	{
		return false;
	}

	if (Credits < Amount)
	{
		// Not enough credits
		return false;
	}
	
	Credits -= Amount;

	OnCreditsChanged.Broadcast(this, Credits, Amount);

	return true;
}

void ASPlayerState::OnRep_Credits(const int32 OldCredits)
{
	OnCreditsChanged.Broadcast(this, Credits, Credits - OldCredits);
}

void ASPlayerState::SavePlayerState_Implementation(USSaveGame* SaveObject)
{
	if (SaveObject)
	{
		// Gather all relevant player data
		FPlayerSaveData SaveData;
		SaveData.Credits = Credits;
		SaveData.PlayerID = GetUniqueId().ToString();

		// May not be alive when we save
		if (const APawn* Pawn = GetPawn())
		{
			SaveData.Location = Pawn->GetActorLocation();
			SaveData.Rotation = Pawn->GetActorRotation();
			SaveData.bResumeAtTransform = true;
		}

		SaveObject->SavedPlayers.Add(SaveData);
	}
}

void ASPlayerState::LoadPlayerState_Implementation(USSaveGame* SaveObject)
{
	if (SaveObject)
	{
		FPlayerSaveData* FoundData = SaveObject->GetPlayerData(this);
		if (FoundData)
		{
			AddCredits(FoundData->Credits);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Could not find SaveGame data for player id '%i'."), GetPlayerId());
		}
	}
}

void ASPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPlayerState, Credits);
}