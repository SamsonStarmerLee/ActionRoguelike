// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SPickupActor.generated.h"

class UCapsuleComponent;

UCLASS()
class ACTIONROGUELIKE_API ASPickupActor : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()

public:	
	ASPickupActor();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCapsuleComponent* CapsuleComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float RespawnTime;

	FTimerHandle TimerHandle_RespawnTimer;

	UFUNCTION()
	void HideAndStartCooldown();

	UFUNCTION()
	void Respawn() const;

public:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	
};