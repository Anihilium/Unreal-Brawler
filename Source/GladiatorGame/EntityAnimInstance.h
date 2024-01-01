// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Entity.h"
#include "EntityAnimInstance.generated.h"

/**
 * 
 */
UCLASS(transient, Blueprintable, hideCategories = AnimInstance, BlueprintType)
class UEntityAnimInstance : public UAnimInstance
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        float Speed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        bool IsDead;

    APawn* OwningPawn;
    AEntity* OwningEntity;

    virtual void NativeInitializeAnimation() override;

    virtual void NativeUpdateAnimation(float DeltaTimeX) override;

};