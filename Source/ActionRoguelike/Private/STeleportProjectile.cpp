// Fill out your copyright notice in the Description page of Project Settings.

#include "STeleportProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ASTeleportProjectile::ASTeleportProjectile() : Super()
{
}

void ASTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager()
		.SetTimer(TimerHandle, this, &ASTeleportProjectile::OnDeplete, 0.2f);

	SphereComponent->IgnoreActorWhenMoving(GetInstigator(), true);
}

void ASTeleportProjectile::OnDeplete()
{
	MovementComponent->StopMovementImmediately();

	EffectComponent->Complete();
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DepleteParticles, GetActorLocation(), GetActorRotation());
	
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ASTeleportProjectile::OnTeleport, 0.2f);
}

void ASTeleportProjectile::OnTeleport()
{
	const auto Inst = GetInstigator();
	Inst->SetActorLocation(GetActorLocation());

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TeleportParticle, GetActorLocation(), GetActorRotation());
	
	Destroy();
}