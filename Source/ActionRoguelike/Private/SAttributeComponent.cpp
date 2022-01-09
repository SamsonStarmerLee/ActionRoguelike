// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

USAttributeComponent::USAttributeComponent()
{
	MaxHealth = 100;
	Health = MaxHealth;
}

bool USAttributeComponent::IsFullHealth() const
{
	return Health == MaxHealth;
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0;
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	const float OldHealth = Health;

	Health = FMath::Clamp(Health + Delta, 0.f, MaxHealth);

	const float TrueDelta = Health - OldHealth;
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, TrueDelta);
	
	return TrueDelta != 0;
}