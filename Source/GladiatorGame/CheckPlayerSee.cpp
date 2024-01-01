// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckPlayerSee.h"
#include "EnemyController.h"

void UCheckPlayerSee::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	AEnemyController* m_controller = Cast<AEnemyController>(OwnerComp.GetAIOwner());
	if (!m_controller->CheckObjestacle())
	{
		m_controller->StopMovement();
	}
	
}