// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Enemy.h"
#include "AttackTask.generated.h"
/**
 *
 */

struct FBTAttack
{
	AEnemy* pawn;
	bool hasAttackLaunch = false;
};

UCLASS()
class GLADIATORGAME_API UAttackTask : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UAttackTask(const FObjectInitializer& objectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void  TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float deltaSeconds) override;
	UPROPERTY(EditAnywhere)
		FName StateAttacking = "IsAttacking";
	uint16 GetInstanceMemorySize() const;
private:
	bool hasAttackLaunch = false;
};
