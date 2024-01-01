// Fill out your copyright notice in the Description page of Project Settings.


#include "UpdateLocation.h"
#include "Components/BoxComponent.h"
#include "Enemy.h"
#include "DrawDebugHelpers.h"


void UUpdateLocation::TickNode(UBehaviorTreeComponent& OwnerComp,uint8* NodeMemory,float DeltaSeconds)
{
	Super::TickNode(OwnerComp,NodeMemory,DeltaSeconds);

	AEnemyController* m_controller = Cast<AEnemyController>(OwnerComp.GetAIOwner());
	UBlackboardComponent* m_blackboardComponent = m_controller->GetBlackboardComponent();
	AEnemy* enemy = Cast<AEnemy>(m_blackboardComponent->GetValueAsObject(SelfKey));
	AEnemiesManager* enemiesManager = Cast<AEnemiesManager>(enemy->manager);
	AActor* target = Cast<AActor>(m_blackboardComponent->GetValueAsObject(TargetKey));

	float closeRadius = m_blackboardComponent->GetValueAsFloat(closeEnemiesRadius.SelectedKeyName);
	FVector targetPosition = target->GetActorLocation();
	FVector selfPosition = enemy->GetActorLocation();

	float angleRotation = 0.f;
	FVector direction = CalculateBaseDirection(selfPosition,targetPosition);
	bool isClose=false;
	if (IsCloseEnemies(enemiesManager,enemy,selfPosition,closeRadius))
	{
		//GEngine->AddOnScreenDebugMessage(-1,15.0f,FColor::Cyan,FString::Printf(TEXT("Close")));
		angleRotation += 45.f;
		isClose = true;
	}

	float radius = m_blackboardComponent->GetValueAsFloat(RadiusPositionKey);
	float anglePosMin= m_blackboardComponent->GetValueAsFloat(angleMinPosition.SelectedKeyName);
	bool isDestValide = false;
	FVector directionRotate = direction;
	for(int i = 0; i < 15; ++i)
	{
		directionRotate = direction;
		directionRotate = RotateDirection(directionRotate,angleRotation);
		if (IsDestinationIsValid(enemiesManager,enemy->index,directionRotate,anglePosMin) &&!CheckWall(targetPosition,directionRotate,radius,enemy))
		{
			isDestValide = true;
			break;
		}
		directionRotate = direction;
		directionRotate  =RotateDirection(directionRotate,-angleRotation);
		if (IsDestinationIsValid(enemiesManager,enemy->index,directionRotate,anglePosMin) && !CheckWall(targetPosition,directionRotate,radius,enemy))
		{
			isDestValide=true;
			break;
		}

		angleRotation += anglePosMin;
	}
	FVector destination = targetPosition + directionRotate * radius;
	m_blackboardComponent->SetValueAsVector(DestinationKey,destination);

}

FVector UUpdateLocation::CalculateBaseDirection(FVector selfPosition,FVector targetPosition)
{
	FVector direction = (selfPosition - targetPosition);
	direction.Z = 0.0f;
	direction.Normalize();
	return direction;
}

bool UUpdateLocation::IsCloseEnemies(AEnemiesManager* enemiesManager,AEnemy* enemy,FVector selfLocation,float closeRadius)
{
	TArray<AEnemy*> arrayEnemies = enemiesManager->GetEnemies();

	for (int i = 0; i < arrayEnemies.Num(); i++)
	{
		if (arrayEnemies[i] == enemy) { continue; }
		FVector targetLocation = arrayEnemies[i]->GetActorLocation();
		if (FVector::Dist(selfLocation,targetLocation) < closeRadius)
		{
			return true;
		}
	}
	return false;
}

bool UUpdateLocation::IsDestinationIsValid(AEnemiesManager* enemiesManager,int indexEnemies,FVector direction,float angleMin)
{
	float cos = direction.CosineAngle2D(FVector::ForwardVector);
	float angle = FMath::RadiansToDegrees( FMath::Acos(cos));

	for (size_t i = 0; i < enemiesManager->enemiesAnglePos.Num(); i++)
	{
		if (i == indexEnemies) { continue; }
		float angleDiff = enemiesManager->enemiesAnglePos[i] - angle;
		
		if (FMath::Abs(angleDiff) <= angleMin)
		{
			return false;
		}

	}
	enemiesManager->enemiesAnglePos[indexEnemies] = angle;
	//GEngine->AddOnScreenDebugMessage(-1,15.0f,FColor::Red,FString::Printf(TEXT("DestValid")));
	return true;
}
bool UUpdateLocation::IsPathIsValid(FVector selfPosition,FVector destination,AEnemy* self)
{

	FHitResult hit;

	FVector direction = destination - selfPosition;
	float width = self->radiusSphere;
	FVector end = selfPosition + direction;
	FCollisionQueryParams traceParams;
	traceParams.AddIgnoredActor(self);

	if (GetWorld()->LineTraceSingleByChannel(hit,selfPosition,end + self->GetActorRightVector() * width,ECC_Visibility,traceParams) || GetWorld()->LineTraceSingleByChannel(hit,selfPosition,end - self->GetActorRightVector() * width,ECC_Visibility,traceParams))
	{
		return hit.Actor.Get()->ActorHasTag("Enemi");
	}
	else
	{
		return false;
	}
}


bool UUpdateLocation::CheckWall(FVector start,FVector direction,float radius,AActor* self)
{
	FHitResult hit;

	FVector end = start + direction * (radius);
	FCollisionQueryParams traceParams;
	traceParams.AddIgnoredActor(self);

	if (GetWorld()->LineTraceSingleByChannel(hit,start,end,ECC_Visibility,traceParams))
	{
		return hit.Component.Get()->ComponentHasTag("Wall");
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1,15.0f,FColor::Red,FString::Printf(TEXT("CheckWallFalse")));
		return false;
	}
}

FVector UUpdateLocation::RotateDirection(FVector direction,float angle)
{
	FQuat rot = rot.MakeFromEuler(FVector(0,0,angle));
	direction = rot.RotateVector(direction);
	direction.Z = .0f;
	direction.Normalize();
	return direction;
}
