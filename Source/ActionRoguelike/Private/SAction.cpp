// Fill out your copyright notice in the Description page of Project Settings.

#include "SAction.h"
#include "SActionComponent.h"

bool USAction::IsRunning() const
{
	return bIsRunning;
}

bool USAction::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning())
	{
		return false;
	}
	
	const auto ActionComponent = GetOwningComponent();
	
	if (ActionComponent->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}

	return true;
}

void USAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
	
	ensureAlways(!bIsRunning);
	
	const auto OwningComponent = GetOwningComponent();
	OwningComponent->ActiveGameplayTags.AppendTags(GrantsTags);

	bIsRunning = true;
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));

	ensureAlways(bIsRunning);
	
	const auto OwningComponent = GetOwningComponent();
	OwningComponent->ActiveGameplayTags.RemoveTags(GrantsTags);

	bIsRunning = false;
}

USActionComponent* USAction::GetOwningComponent() const
{
	return Cast<USActionComponent>(GetOuter());
}

UWorld* USAction::GetWorld() const
{
	// Outer is set when creating action via NewObject<>
	const auto OuterActor = Cast<UActorComponent>(GetOuter());
	if (OuterActor)
	{
		return OuterActor->GetWorld();
	}

	return nullptr;
}