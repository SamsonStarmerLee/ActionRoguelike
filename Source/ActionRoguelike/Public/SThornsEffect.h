// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SActionEffect.h"
#include "SAttributeComponent.h"
#include "SThornsEffect.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USThornsEffect : public USActionEffect
{
	GENERATED_BODY()

public:
	virtual void StartAction_Implementation(AActor* Instigator) override;

	virtual void StopAction_Implementation(AActor* Instigator) override;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Thorns")
	float ThornsDamageRatio = 0.1f;

	UFUNCTION()
	void ApplyThornsDamage(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);
};