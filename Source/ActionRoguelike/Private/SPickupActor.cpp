// Fill out your copyright notice in the Description page of Project Settings.

#include "SPickupActor.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"

ASPickupActor::ASPickupActor()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	RootComponent = SceneComponent;
	
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CapsuleComponent->SetupAttachment(RootComponent);
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	MeshComponent->SetupAttachment(RootComponent);

	bIsActive = true;

	// Only use SetReplicates() outside of constructors.
	bReplicates = true;
}

void ASPickupActor::OnRep_IsActive()
{
	SetActorEnableCollision(bIsActive);
	// Set visibility on root and all children
	RootComponent->SetVisibility(bIsActive, true);
}

void ASPickupActor::HideAndStartCooldown()
{
	SetIsActiveState(false);
	
	GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &ASPickupActor::Respawn, RespawnTime);
}

void ASPickupActor::SetIsActiveState(const bool bActive)
{
	bIsActive = bActive;
	OnRep_IsActive();
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

FText ASPickupActor::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	// Implement in derived classes
	return FText::GetEmpty();
}

void ASPickupActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPickupActor, bIsActive);
}