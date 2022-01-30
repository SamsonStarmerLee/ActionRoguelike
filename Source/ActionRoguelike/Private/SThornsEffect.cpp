// Fill out your copyright notice in the Description page of Project Settings.


#include "SThornsEffect.h"
#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SGameplayFunctionLibrary.h"

void USThornsEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	const auto AttributeComponent = USAttributeComponent::GetAttributes(Instigator);
	if (AttributeComponent)
	{
		AttributeComponent->OnHealthChanged.AddDynamic(this, &USThornsEffect::ApplyThornsDamage);
	}
}

void USThornsEffect::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	const auto AttributeComponent = USAttributeComponent::GetAttributes(Instigator);
	if (AttributeComponent)
	{
		AttributeComponent->OnHealthChanged.RemoveDynamic(this, &USThornsEffect::ApplyThornsDamage);
	}
}

void USThornsEffect::ApplyThornsDamage(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	AActor* OwningActor = GetOwningComponent()->GetOwner();

	if (Delta < 0.f && OwningActor != InstigatorActor)
	{
		int32 Damage = FMath::RoundToInt(Delta * ThornsDamageRatio);
		if (Damage == 0)
		{
			return;	
		}

		// Flip damage amount to positive (that's how ApplyDamage wants it delivered)
		Damage = FMath::Abs(Damage);
		
		USGameplayFunctionLibrary::ApplyDamage(OwningActor, InstigatorActor, Damage);
	}
}