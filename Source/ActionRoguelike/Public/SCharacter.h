// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

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
	UAnimMontage* AttackAnim;

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

	virtual void BeginPlay() override;

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

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};