// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "EnemyController.h"
#include "EnemiesManager.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "UpdateLocation.generated.h"

/**
 * 
 */
UCLASS()
class GLADIATORGAME_API UUpdateLocation : public UBTService
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere)
		FName DestinationKey;
	UPROPERTY(EditAnywhere)
		FName TargetKey;
	UPROPERTY(EditAnywhere)
		FName SelfKey;
	UPROPERTY(EditAnywhere)
		FName RadiusPositionKey;
	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector closeEnemiesRadius;
	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector angleMinPosition;
	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector distanceSideMovement;

protected :
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	bool CheckWall(FVector start, FVector direction, float radius, AActor* self);
	FVector CalculateBaseDirection(FVector selfPosition,FVector targetPosition);
	bool IsCloseEnemies(AEnemiesManager* enemiesManager,AEnemy* enemy,FVector selfLocation,float closeRadius);
	bool IsDestinationIsValid(AEnemiesManager* enemiesManager,int indexEnemies,FVector direction,float angleMin);
	bool IsPathIsValid(FVector selfPosition,FVector destination,AEnemy* self);
	FVector RotateDirection(FVector direction,float angle);
};
