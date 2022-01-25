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
}

void ASPickupActor::Interact_Implementation(APawn* InstigatorPawn)
{
	// Implement in derived classes
}