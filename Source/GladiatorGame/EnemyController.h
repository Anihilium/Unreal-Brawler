// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyController.generated.h"

/**
 *
 */
UCLASS()
class GLADIATORGAME_API AEnemyController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyController();
	virtual void OnPossess(APawn* InPawn) override;
	void SetEnemyKeyValue(APawn* player,float  radiusPos,float angleMinPos,float closeCounterRadius,float ValidAngleScalar,float radiusAttack);
	bool SetAttackState();
	void ResetAttackState();
	void EndBehaviorTree();
	bool GetAttackState();
	bool CheckObjestacle();
	bool IsCloseEnoughToAttack();

protected:

		UAISenseConfig_Sight* sight;
	UPROPERTY(EditAnywhere,Category = AI)
		UBehaviorTreeComponent* m_behaviorTreeComponent;
	UPROPERTY(EditAnywhere,Category = AI)
		UBlackboardComponent* m_blackboardComponent;
	UPROPERTY(EditAnywhere,Category = AI)
		UBehaviorTree* m_behaviorTree;

	virtual void  Tick(float DeltaTime) override;
	bool value;

private :
	bool CheckEnemiesDistancePlayer();
	APawn* m_player;
	APawn* m_selfPawn;
	float m_radius;
	float m_attackRadius;
};
