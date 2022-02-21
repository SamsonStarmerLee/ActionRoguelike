// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPickupActor.h"
#include "SHealthPotion.generated.h"

class UCapsuleComponent;

UCLASS()
class ACTIONROGUELIKE_API ASHealthPotion final : public ASPickupActor
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditAnywhere)
	int32 Cost;
	
	UPROPERTY(EditAnywhere)
	float Heal;

public:	
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	virtual FText GetInteractText_Implementation(APawn* InstigatorPawn) override;
};