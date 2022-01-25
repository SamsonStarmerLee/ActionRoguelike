// Fill out your copyright notice in the Description page of Project Settings.

#include "SHealthPotion.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "SPlayerState.h"
#include "Components/CapsuleComponent.h"

void ASHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}
	
	const auto PlayerState = InstigatorPawn->GetPlayerState<ASPlayerState>();
	if (!PlayerState)
	{
		return;
	}

	if (PlayerState->GetCredits() < Cost)
	{
		// Can't afford
		return;
	}

	const auto Attributes = InstigatorPawn->FindComponentByClass<USAttributeComponent>();
	if (!Attributes)
	{
		return;
	}

	if (!Attributes->IsFullHealth())
	{
		// Full health, can't heal
		return;
	}

	// Pay for heal
	PlayerState->RemoveCredits(Cost);
	
	FTimerHandle RespawnTimer;
	GetWorldTimerManager().SetTimer(RespawnTimer, this, &ASHealthPotion::Respawn, RespawnCooldown);

	// Heal!
	Attributes->ApplyHealthChange(this, Heal);

	// Disable while on cooldown
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent->SetVisibility(false, true);
}

void ASHealthPotion::Respawn() const
{
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	RootComponent->SetVisibility(true, true);
}