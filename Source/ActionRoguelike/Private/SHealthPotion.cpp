// Fill out your copyright notice in the Description page of Project Settings.

#include "SHealthPotion.h"

#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "Components/CapsuleComponent.h"

ASHealthPotion::ASHealthPotion()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	RootComponent = SceneComponent;
	
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("SphereComponent");
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CapsuleComponent->SetupAttachment(RootComponent);
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	MeshComponent->SetupAttachment(RootComponent);
}

void ASHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	const auto Attributes = InstigatorPawn->FindComponentByClass<USAttributeComponent>();

	if (Attributes && !Attributes->IsFullHealth())
	{
		FTimerHandle RespawnTimer;
		GetWorldTimerManager().SetTimer(RespawnTimer, this, &ASHealthPotion::Respawn, RespawnCooldown);

		// Heal!
		Attributes->ApplyHealthChange(this, Heal);

		// Disable while on cooldown
		CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		RootComponent->SetVisibility(false, true);
	}
}

void ASHealthPotion::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASHealthPotion::Respawn()
{
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	RootComponent->SetVisibility(true, true);
}