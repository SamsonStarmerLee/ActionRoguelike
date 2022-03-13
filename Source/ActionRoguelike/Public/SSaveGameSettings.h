// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/DeveloperSettings.h"
#include "SSaveGameSettings.generated.h"

// 'defaultconfig' = "Save object config only to Default INIs, never to local INIs."
UCLASS(
	Config=Game,
	defaultconfig,
	meta = (DisplayName="Save Game Settings")) // This will appear as a category in project settings
class ACTIONROGUELIKE_API USSaveGameSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "General")
	FString SaveSlotName;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "General", AdvancedDisplay)
	TSoftObjectPtr<UDataTable> DummyTablePath;

	USSaveGameSettings();
};