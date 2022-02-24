// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_STATS_GROUP(TEXT("SAMMY_Game"), STATGROUP_SAMMY, STATCAT_Advanced); 

static void LogOnScreen(UObject* WorldContext, FString Msg, FColor Color = FColor::White, float Duration = 5.0f)
{
	if (!ensure(WorldContext))
	{
		return;
	}

	const UWorld* World = WorldContext->GetWorld();
	if (!ensure(World))
	{
		return;
	}

	const FString NetPrefix = World->IsNetMode(NM_Client) ? "[CLIENT]" : "[SERVER]";
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, Duration, Color, NetPrefix + Msg);
	}
}