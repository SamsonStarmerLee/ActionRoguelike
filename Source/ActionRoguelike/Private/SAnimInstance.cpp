// Fill out your copyright notice in the Description page of Project Settings.


#include "SAnimInstance.h"

#include "GameplayTagContainer.h"
#include "SActionComponent.h"

void USAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	const auto OwningActor = GetOwningActor();
	if (OwningActor)
	{
		ActionComponent = OwningActor->FindComponentByClass<USActionComponent>();
	}
}

void USAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	static FGameplayTag StunnedTag = FGameplayTag::RequestGameplayTag("Status.Stunned");
	if (ActionComponent)
	{
		bIsStunned = ActionComponent->ActiveGameplayTags.HasTag(StunnedTag);
	}
}