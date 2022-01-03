// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class UPawnSensingComponent;

UCLASS()
class ACTIONROGUELIKE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASAICharacter();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComponent;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);
};