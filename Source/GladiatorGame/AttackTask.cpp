// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackTask.h"
#include "EnemyController.h"

#include "BehaviorTree/Tasks/BTTask_MoveTo.h"


uint16 UAttackTask::GetInstanceMemorySize() const
{
	return sizeof(FBTAttack);
}
UAttackTask::UAttackTask(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer)
{
	bNotifyTick = true;
}
EBTNodeResult::Type UAttackTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyController* m_controller = Cast<AEnemyController>(OwnerComp.GetAIOwner());
	UBlackboardComponent* m_blackboardComponent = m_controller->GetBlackboardComponent();

	FBTAttack* memory = reinterpret_cast<FBTAttack*>(NodeMemory);

	memory->pawn = Cast<AEnemy>(m_controller->GetPawn());
	memory->pawn->Attack();
	return EBTNodeResult::InProgress;
}

void UAttackTask::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)	
{
	AEnemyController* m_controller = Cast<AEnemyController>(OwnerComp.GetAIOwner());
	UBlackboardComponent* m_blackboardComponent = m_controller->GetBlackboardComponent();
	FBTAttack* memory = reinterpret_cast<FBTAttack*>(NodeMemory);


	if (memory->pawn->IsAttacking)
	{
		return;
	}
	else
	{
		m_blackboardComponent->SetValueAsBool(StateAttacking, false);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}