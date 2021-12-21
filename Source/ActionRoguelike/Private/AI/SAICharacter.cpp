// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"

ASAICharacter::ASAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ASAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}