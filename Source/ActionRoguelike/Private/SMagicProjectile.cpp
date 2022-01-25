// Fill out your copyright notice in the Description page of Project Settings.

#include "SMagicProjectile.h"

#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SGameplayFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ASMagicProjectile::ASMagicProjectile() : Super()
{
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap);
}

void ASMagicProjectile::OnActorOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult & SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		const auto ActionComponent = OtherActor->FindComponentByClass<USActionComponent>();
		if (ActionComponent && ActionComponent->ActiveGameplayTags.HasTag(ParryTag))
		{
			// Invert velocity for getting 'batted back' when parried
			MovementComponent->Velocity = -MovementComponent->Velocity;

			SetInstigator(Cast<APawn>(OtherActor));
			return;
		}

		if (USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, Damage, SweepResult))
		{
			Explode();
		}
	}
}

void ASMagicProjectile::Explode_Implementation()
{
	UGameplayStatics::PlayWorldCameraShake(this, CameraShake, GetActorLocation(), InnerShakeRadius, OuterShakeRadius);

	Super::Explode_Implementation();
}