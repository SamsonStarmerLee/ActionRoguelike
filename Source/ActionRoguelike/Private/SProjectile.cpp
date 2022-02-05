// Fill out your copyright notice in the Description page of Project Settings.

#include "SProjectile.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ASProjectile::ASProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetCollisionProfileName("Projectile");
	RootComponent = SphereComponent;

	EffectComponent = CreateDefaultSubobject<UParticleSystemComponent>("EffectComponent");
	EffectComponent->SetupAttachment(RootComponent);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->bInitialVelocityInLocalSpace = true;

	FlightAudioComponent = CreateDefaultSubobject<UAudioComponent>("FlightAudioComponent");
	FlightAudioComponent->SetupAttachment(RootComponent);

	SetReplicates(true);
}

void ASProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComponent->IgnoreActorWhenMoving(GetInstigator(), true);
	SphereComponent->OnComponentHit.AddDynamic(this, &ASProjectile::OnActorHit);
}

void ASProjectile::OnActorHit(
	UPrimitiveComponent* HitComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	Explode();
}

void ASProjectile::Explode_Implementation()
{
	// Check to make sure we aren't already being 'destroyed'
	if (ensure(!IsPendingKill()))
	{
		if (ImpactVFX)
		{
			UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());
		}

		if (ImpactSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, GetActorLocation(), GetActorRotation());
		}
	
		Destroy();
	}
}