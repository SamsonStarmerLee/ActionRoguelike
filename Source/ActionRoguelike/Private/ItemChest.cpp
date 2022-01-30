// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemChest.h"
#include "Net/UnrealNetwork.h"

AItemChest::AItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>("BodyMesh");
	RootComponent = BodyMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(RootComponent);

	TargetPitch = 110;

	SetReplicates(true);
}

void AItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	bLidOpened = !bLidOpened;
	OnRep_LidOpened(); // Manually call for server (if desired. rep_notifies don't auto-trigger serverside.)
}

void AItemChest::OnRep_LidOpened()
{
	const float CurrentPitch = bLidOpened ? TargetPitch : 0;
	LidMesh->SetRelativeRotation(FRotator(CurrentPitch, 0 ,0));
}

void AItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AItemChest, bLidOpened);
}