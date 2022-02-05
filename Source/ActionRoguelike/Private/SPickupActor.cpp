// Fill out your copyright notice in the Description page of Project Settings.

#include "SPickupActor.h"
#include "Components/CapsuleComponent.h"

ASPickupActor::ASPickupActor()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	RootComponent = SceneComponent;
	
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CapsuleComponent->SetupAttachment(RootComponent);
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	MeshComponent->SetupAttachment(RootComponent);

	SetReplicates(true);
}

void ASPickupActor::HideAndStartCooldown()
{
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent->SetVisibility(false, true);

	GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &ASPickupActor::Respawn, RespawnTime);
}

void ASPickupActor::Respawn() const
{
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	RootComponent->SetVisibility(true, true);
}

void ASPickupActor::Interact_Implementation(APawn* InstigatorPawn)
{
	// Implement in derived classes
}