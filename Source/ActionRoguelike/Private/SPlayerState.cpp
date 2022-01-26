// Fill out your copyright notice in the Description page of Project Settings.

#include "SPlayerState.h"

int32 ASPlayerState::GetCredits() const
{
	return Credits;
}

void ASPlayerState::AddCredits(const int32 Amount)
{
	if (!ensure(Amount > 0))
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