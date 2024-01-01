// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CheckPlayerSee.generated.h"

/**
 * 
 */
UCLASS()
class GLADIATORGAME_API UCheckPlayerSee : public UBTService
{
	GENERATED_BODY()
protected:
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
