// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "EnemyController.h"
#include "ChooseDestination.generated.h"

/**
 *
 */
UCLASS()
class GLADIATORGAME_API UChooseDestination : public UBTTaskNode
{
	GENERATED_BODY()
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	UPROPERTY(EditAnywhere)
		FName DestinationKey;
	UPROPERTY(EditAnywhere)
		FName TargetKey;
	UPROPERTY(EditAnywhere)
		FName SelfKey;
	UPROPERTY(EditAnywhere)
		FName RadiusPositionKey;

protected:
	bool CheckWall(FVector start, FVector direction, float radius, AActor* self);
};
