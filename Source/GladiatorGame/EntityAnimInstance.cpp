// Fill out your copyright notice in the Description page of Project Settings.


#include "EntityAnimInstance.h"

//This function is like PostInitAnimtree in UE3
void UEntityAnimInstance::NativeInitializeAnimation()
{
    //Very Important Line
    Super::NativeInitializeAnimation();

    //Cache the owning pawn for use in Tick
    OwningPawn = TryGetPawnOwner();
    OwningEntity = Cast<AEntity, APawn>(OwningPawn);
    IsDead = false;
}

//Tick
void UEntityAnimInstance::NativeUpdateAnimation(float DeltaTimeX)
{
    //Very Important Line
    Super::NativeUpdateAnimation(DeltaTimeX);

    //Always Check Pointers
    if (IsDead || !OwningPawn)
        return;

    Speed = OwningPawn->GetVelocity().Size() / 6.f;

    IsDead = OwningEntity->Life < 1;
}