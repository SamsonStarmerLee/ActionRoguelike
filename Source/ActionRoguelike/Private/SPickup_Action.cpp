// Fill out your copyright notice in the Description page of Project Settings.

#include "SPickup_Action.h"
#include "SAction.h"
#include "SActionComponent.h"

void ASPickup_Action::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn && ActionToGrant))
	{
		return;
	}

	const auto ActionComponent = InstigatorPawn->FindComponentByClass<USActionComponent>();
	if (ActionComponent)
	{
		if (ActionComponent->GetAction(ActionToGrant))
		{
			const FString DebugMsg = FString::Printf(TEXT("Action '%s' already known."), *GetNameSafe(ActionToGrant));
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, DebugMsg);
			return;
		}

		// Give new ability and go on cooldown
		ActionComponent->AddAction(this, ActionToGrant);
		HideAndStartCooldown();
	}
}