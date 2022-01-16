// Fill out your copyright notice in the Description page of Project Settings.

#include "BTS_CheckLowHealth.h"
#include "AIController.h"
#include "SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

void UBTS_CheckLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Check for low character health.

	const auto AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ensure(AIPawn))
	{
		const auto AttributeComponent = USAttributeComponent::GetAttributes(AIPawn);
		if (ensure(AttributeComponent))
		{
			const bool bLowHealth = AttributeComponent->GetHealth() / AttributeComponent->GetMaxHealth() <= LowHealthFraction;

			const auto BlackboardComponent = OwnerComp.GetBlackboardComponent();
			BlackboardComponent->SetValueAsBool(IsLowHealthKey.SelectedKeyName, bLowHealth);
		}
	}
}