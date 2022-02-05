// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/SAICharacter.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "DrawDebugHelpers.h"
#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SWorldUserWidget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"

ASAICharacter::ASAICharacter()
{
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	AttributeComponent = CreateDefaultSubobject<USAttributeComponent>("AttributeComponent");
	ActionComponent = CreateDefaultSubobject<USActionComponent>("ActionComponent");

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	TimeToHitParamName = "TimeOfHit";
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
	AttributeComponent->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);
}

void ASAICharacter::SetTargetActor(AActor* NewTarget) const
{
	const auto AiController = Cast<AAIController>(GetController());
	if (AiController)
	{
		AiController->
			GetBlackboardComponent()->
			SetValueAsObject("TargetActor", NewTarget);
	}
}

void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		if (InstigatorActor != this)
		{
			SetTargetActor(InstigatorActor);
		}

		// Spawn damage-display healthbar when hit
		if (!ActiveHealthBar)
		{
			ActiveHealthBar = CreateWidget<USWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if (ActiveHealthBar)
			{
				ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport();
			}
		}

		// Hit flash
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);

		// Died
		if (NewHealth <= 0.0f)
		{
			// Stop BT
			const auto AiController = Cast<AAIController>(GetController());
			if (AiController)
			{
				AiController->GetBrainComponent()->StopLogic("Killed");
			}
			
			// Ragdoll
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");

			// Disable collision
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();
			
			// Set Lifespan
			SetLifeSpan(10.0f);
		}
	}
}

AActor* ASAICharacter::GetTargetActor() const
{
	const auto AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		return Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject(TargetActorKey));
	}

	return nullptr;
}

void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
	// Ignore if target already set to this
	if (GetTargetActor() != Pawn)
	{
		SetTargetActor(Pawn);

		const auto Widget = CreateWidget<USWorldUserWidget>(GetWorld(), SpottedWidgetClass);
		if (Widget)
		{
			Widget->AttachedActor = Pawn;
			Widget->AddToViewport(10); // High index ensures placement over other widgets like healthbars.
		}
	}
}