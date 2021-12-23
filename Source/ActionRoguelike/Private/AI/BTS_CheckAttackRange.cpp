// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTS_CheckAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UBTS_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Check distance between AI pawn and target actor.

	const auto BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackboardComp))
	{
		const auto TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor"));
		if (TargetActor)
		{
			const auto MyController = OwnerComp.GetAIOwner();
			if (ensure(MyController))
			{
				const auto AIPawn = MyController->GetPawn();
				if (ensure(AIPawn))
				{
					const float DistanceTo = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());
					const bool bWithinRange = DistanceTo < 2000.f;
					if (bWithinRange)
					{
						const bool bLineOfSight = MyController->LineOfSightTo(TargetActor);
						BlackboardComp->SetValueAsBool("WithinAttackRange", bLineOfSight);
					}
				}
			}
		}
	}
}