// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

USAttributeComponent::USAttributeComponent()
{
	MaxHealth = 100;
	Health = MaxHealth;
}

float USAttributeComponent::GetHealth() const
{
	return Health;
}

float USAttributeComponent::GetMaxHealth() const
{
	return MaxHealth;
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
	if (!GetOwner()->CanBeDamaged())
	{
		return false;
	}
	
	const float OldHealth = Health;

	Health = FMath::Clamp(Health + Delta, 0.f, MaxHealth);

	const float TrueDelta = Health - OldHealth;
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, TrueDelta);
	
	return TrueDelta != 0;
}

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return FromActor->FindComponentByClass<USAttributeComponent>();
	}

	return nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
	if (const auto AttributeComp = GetAttributes(Actor))
	{
		return AttributeComp->IsAlive();
	}

	return false;
}

bool USAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -MaxHealth);
}