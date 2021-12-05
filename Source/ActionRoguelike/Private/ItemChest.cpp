// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemChest.h"

// Sets default values

AItemChest::AItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>("BodyMesh");
	RootComponent = BodyMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(RootComponent);

	TargetPitch = 110;
}

void AItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	LidMesh->SetRelativeRotation(FRotator(TargetPitch, 0 ,0));
}