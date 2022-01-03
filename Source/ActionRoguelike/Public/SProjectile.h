// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class ACTIONROGUELIKE_API ASProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ASProjectile();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProjectileMovementComponent* MovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* EffectComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAudioComponent* FlightAudioComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UParticleSystem* ImpactVFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* ImpactSound;

	UFUNCTION()
	virtual void OnActorHit(UPrimitiveComponent* HitComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit);

	virtual void PostInitializeComponents() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Explode();
};