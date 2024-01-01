// Fill out your copyright notice in the Description page of Project Settings.


#include "ChooseDestination.h"
#include "Enemy.h"

EBTNodeResult::Type UChooseDestination::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	AEnemyController* m_controller = Cast<AEnemyController>(OwnerComp.GetAIOwner());
	UBlackboardComponent* m_blackboardComponent = m_controller->GetBlackboardComponent();

	AEnemy* enemy = Cast<AEnemy>(m_blackboardComponent->GetValueAsObject(SelfKey));

	FVector position = enemy->GetActorLocation();
	FVector targetPosition = Cast<AActor>(m_blackboardComponent->GetValueAsObject(TargetKey))->GetActorLocation();
	FVector  dir = (position - targetPosition);

	float radius = dir.Size()> enemy->RadiusPositionningMax ? enemy->RadiusPositionningMax: dir.Size();
	float angle = 25;
	dir.Normalize();
	FVector dirRotate = dir;
	bool isValidPos = false;
	while(!isValidPos)
	{
		dirRotate = dir;
		FQuat rot = rot.MakeFromEuler(FVector(0, 0, angle));
		dirRotate = rot.RotateVector(dirRotate);
		dirRotate.Normalize();

		if (CheckWall(targetPosition, dirRotate, radius, m_controller->GetPawn()))
		{
			isValidPos = true;
			break;
		}
		dirRotate = dir;
		rot = rot.MakeFromEuler(FVector(0, 0, -angle));
		dirRotate = rot.RotateVector(dirRotate);
		dirRotate.Normalize();
		if (CheckWall(targetPosition, dirRotate, radius, m_controller->GetPawn()))
		{
			isValidPos = true;
		}
		angle +=25;
	}


	m_blackboardComponent->SetValueAsVector(DestinationKey, targetPosition + dirRotate * radius);

	return EBTNodeResult::Succeeded;
}
bool UChooseDestination::CheckWall(FVector start, FVector direction, float radius, AActor* self)
{
	FHitResult hit;

	direction.Z = 0.0f;
	direction.Normalize();

	FVector end = start + direction * (radius + 150);
	FCollisionQueryParams traceParams;
	traceParams.AddIgnoredActor(self);
		
	if (GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Visibility, traceParams))
	{
		if (hit.Component.Get()->ComponentHasTag("Wall"))
		{
			direction.Z = 0.0f;
			direction.Z = 0.0f;
			direction.Normalize();
			return true;
		}
		if (hit.Actor.Get()->ActorHasTag("Enemy"))
		{
			return true;
		}
		return false;

	}
	else
	{
		return false;
	}
}