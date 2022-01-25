// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPickupActor.h"
#include "SCoin.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASCoin final : public ASPickupActor
{
	GENERATED_BODY()

public:
	ASCoin();

protected:

	UPROPERTY(EditAnywhere, Category = "Credits")
	int32 CreditValue = 1;

public:

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
};