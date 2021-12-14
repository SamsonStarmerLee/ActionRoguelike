// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectile.h"
#include "STeleportProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASTeleportProjectile : public ASProjectile
{
	GENERATED_BODY()

protected:

	FTimerHandle TimerHandle;

	UPROPERTY(EditAnywhere)
	UParticleSystem* DepleteEffect;

	UPROPERTY(EditAnywhere)
	UParticleSystem* TeleportEffect;

public:

	ASTeleportProjectile();

protected:

	virtual void BeginPlay() override;

	void OnDeplete();

	void OnTeleport();
	
};