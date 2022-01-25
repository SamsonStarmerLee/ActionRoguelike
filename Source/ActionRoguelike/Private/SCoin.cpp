// Fill out your copyright notice in the Description page of Project Settings.

#include "SCoin.h"

#include "SPlayerState.h"

ASCoin::ASCoin()
{
}

void ASCoin::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}
	
	const auto PlayerState = InstigatorPawn->GetPlayerState<ASPlayerState>();
	if (PlayerState)
	{
		PlayerState->AddCredits(CreditValue);
		Destroy();
	}
}