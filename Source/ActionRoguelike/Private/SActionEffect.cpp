// Fill out your copyright notice in the Description page of Project Settings.

#include "SActionEffect.h"

#include "SActionComponent.h"
#include "GameFramework/GameStateBase.h"

USActionEffect::USActionEffect()
{
	bAutoStart = true;
}

float USActionEffect::GetTimeRemaining() const
{
	AGameStateBase* GameState = GetWorld()->GetGameState<AGameStateBase>();
	if (GameState)
	{
		const float EndTime = TimeStarted + Duration;
		return EndTime - GameState->GetServerWorldTimeSeconds();
	}

	return Duration;
}

void USActionEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	if (Duration > 0.f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "StopAction", Instigator);
		GetWorld()->GetTimerManager().SetTimer(DurationHandle, Delegate, Duration, false);
	}

	if (Period > 0.f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ExecutePeriodicEffect", Instigator);
		GetWorld()->GetTimerManager().SetTimer(PeriodHandle, Delegate, Period, true);
	}
}

void USActionEffect::StopAction_Implementation(AActor* Instigator)
{
	if (GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(Instigator);
	}
	
	Super::StopAction_Implementation(Instigator);

	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);
	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);

	if (ActionComponent)
	{
		ActionComponent->RemoveAction(this);
	}
}

void USActionEffect::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{
		
}