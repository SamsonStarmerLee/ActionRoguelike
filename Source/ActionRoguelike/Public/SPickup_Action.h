// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPickupActor.h"
#include "SPickup_Action.generated.h"

class USAction;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASPickup_Action : public ASPickupActor
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USAction> ActionToGrant;
	
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
};