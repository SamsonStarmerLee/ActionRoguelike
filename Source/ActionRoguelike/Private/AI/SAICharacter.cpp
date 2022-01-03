// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/SAICharacter.h"
#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"

ASAICharacter::ASAICharacter()
{
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
}

void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
	const auto AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		const auto BBComp = AIC->GetBlackboardComponent();
		BBComp->SetValueAsObject("TargetActor", Pawn);

		DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::Red, 4.0f, true);
	}
}