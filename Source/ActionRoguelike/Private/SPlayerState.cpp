// Fill out your copyright notice in the Description page of Project Settings.

#include "SPlayerState.h"

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
		SaveObject->Credits = Credits;
	}
}

void ASPlayerState::LoadPlayerState_Implementation(USSaveGame* SaveObject)
{
	if (SaveObject)
	{
		AddCredits(SaveObject->Credits);
	}
}

void ASPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPlayerState, Credits);
}