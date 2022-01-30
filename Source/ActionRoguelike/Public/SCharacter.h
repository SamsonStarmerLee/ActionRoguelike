// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class USActionComponent;
class USAttributeComponent;
class USInteractionComponent;
class UCameraComponent;
class USpringArmComponent;

class ASCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FOnRageChanged,
	ASCharacter*, PlayerCharacter,
	float, NewRage,
	float, Delta);

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASCharacter();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
	USAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actions")
	USActionComponent* ActionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rage")
	float Rage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rage")
	float MaxRage = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rage", meta = (ClampMin=0.f, ClampMax=1.f))
	float RageConversionRate = 0.1f;

	UPROPERTY(BlueprintAssignable, Category = "Rage")
	FOnRageChanged OnRageChanged;
	
	void MoveForward(float Value);

	void MoveRight(float Value);

	void SprintStart();

	void SprintStop();

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

	virtual FVector GetPawnViewLocation() const override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Exec)
	void HealSelf(float Amount = 100);

	UFUNCTION(BlueprintCallable)
	bool SpendRage(float Amount);
};