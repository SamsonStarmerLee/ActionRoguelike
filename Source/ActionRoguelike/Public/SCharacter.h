// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class USAttributeComponent;
class USInteractionComponent;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> PrimaryAttackClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> SecondaryAttackClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> TeleportProjectileClass;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* AttackEffect;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName HandSocketName = "Muzzle_01";
	
	FTimerHandle TimerHandle_Attack;

	FTimerDelegate TimerDelegate_Attack;

public:
	ASCharacter();

protected:

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
	USAttributeComponent* AttributeComponent;
	
	void MoveForward(float Value);

	void MoveRight(float Value);

	UFUNCTION()
	void FireProjectile(TSubclassOf<AActor> ProjectileClass);

	UFUNCTION()
	void PrimaryAttack();

	UFUNCTION()
	void SecondaryAttack();

	UFUNCTION()
	void Teleport();

	UFUNCTION()
	void PrimaryInteract();

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

	virtual void PostInitializeComponents() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};