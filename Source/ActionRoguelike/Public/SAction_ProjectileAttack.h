// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SAction_ProjectileAttack.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API USAction_ProjectileAttack : public USAction
{
	GENERATED_BODY()

public:
	USAction_ProjectileAttack();

protected:
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName HandSocketName = "Muzzle_01";

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float AttackAnimDelay;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* CastAnim;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* CastEffect;

	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);

public:

	virtual void StartAction_Implementation(AActor* Instigator) override;
};