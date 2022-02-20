// Fill out your copyright notice in the Description page of Project Settings.

#include "SAction.h"
#include "SActionComponent.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "Net/UnrealNetwork.h"

void USAction::Initialize(USActionComponent* NewActionComp)
{
	ActionComponent = NewActionComp;
}

bool USAction::IsRunning() const
{
	return RepData.bIsRunning;
}

bool USAction::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning())
	{
		return false;
	}
	
	if (ActionComponent->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}

	return true;
}

void USAction::StartAction_Implementation(AActor* Instigator)
{
	// UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
	// LogOnScreen(this, FString::Printf(TEXT("Started %s"), *ActionName.ToString()), FColor::Green);
	
	const auto OwningComponent = GetOwningComponent();
	OwningComponent->ActiveGameplayTags.AppendTags(GrantsTags);

	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;

	if (GetOwningComponent()->GetOwnerRole() == ROLE_Authority)
	{
		TimeStarted = GetWorld()->TimeSeconds;
	}

	GetOwningComponent()->OnActionStarted.Broadcast(GetOwningComponent(), this);
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	// UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));
	// LogOnScreen(this, FString::Printf(TEXT("Stopped %s"), *ActionName.ToString()), FColor::White);
	
	const auto OwningComponent = GetOwningComponent();
	OwningComponent->ActiveGameplayTags.RemoveTags(GrantsTags);

	RepData.bIsRunning = false;
	RepData.Instigator = Instigator;

	GetOwningComponent()->OnActionStopped.Broadcast(GetOwningComponent(), this);
}

USActionComponent* USAction::GetOwningComponent() const
{
	return ActionComponent;
}

void USAction::OnRep_RepData()
{
	if (RepData.bIsRunning)
	{
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}

UWorld* USAction::GetWorld() const
{
	// Outer is set when creating action via NewObject<>
	const AActor* OuterActor = Cast<AActor>(GetOuter());
	if (OuterActor)
	{
		return OuterActor->GetWorld();
	}

	return nullptr;
}

void USAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAction, RepData);
	DOREPLIFETIME(USAction, ActionComponent);
	DOREPLIFETIME(USAction, TimeStarted);
}