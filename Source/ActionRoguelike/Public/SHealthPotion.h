// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SHealthPotion.generated.h"

class UCapsuleComponent;

UCLASS()
class ACTIONROGUELIKE_API ASHealthPotion : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCapsuleComponent* CapsuleComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere)
	float Heal;

public:	
	ASHealthPotion();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void Respawn();

public:	
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
};