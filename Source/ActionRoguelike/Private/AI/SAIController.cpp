// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/SAIController.h"
#include "Kismet/GameplayStatics.h"

void ASAIController::BeginPlay()
{
	Super::BeginPlay();

	if (ensureMsgf(BehaviorTree, TEXT("Behavior Tree is nullptr! Please assign behavior tree in your AI controller.")))
	{
		RunBehaviorTree(BehaviorTree);
	}

	// const auto PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	// if (PlayerPawn)
	// {
	// 	GetBlackboardComponent()->
	// 		SetValueAsVector("MoveToLocation", PlayerPawn->GetActorLocation());
	//
	// 	GetBlackboardComponent()->
	// 		SetValueAsObject("TargetActor", PlayerPawn);
	// }
}