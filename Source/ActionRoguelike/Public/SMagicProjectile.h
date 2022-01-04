// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectile.h"
#include "SMagicProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class ACTIONROGUELIKE_API ASMagicProjectile : public ASProjectile
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera Shake")
	TSubclassOf<UCameraShakeBase> CameraShake;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera Shake")
	float InnerShakeRadius = 200.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera Shake")
	float OuterShakeRadius = 1000.f;
	
	UFUNCTION()
	virtual void OnActorOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult & SweepResult);

	virtual void Explode_Implementation() override;
	
public:
	ASMagicProjectile();
};