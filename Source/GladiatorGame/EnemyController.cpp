// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyController.h"
#include "DrawDebugHelpers.h"
#include "Enemy.h"
#include "MyPlayer.h"

AEnemyController::AEnemyController()
{
	m_behaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behavior Component"));
	m_blackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Black Board Component"));
}

void AEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	AEnemy* AIEnemy = Cast<AEnemy>(InPawn);
	if (AIEnemy)
	{
		if (AIEnemy->m_behaviorTree->GetBlackboardAsset())
		{
			m_blackboardComponent->InitializeBlackboard(*(AIEnemy->m_behaviorTree->BlackboardAsset));
		}
	}
	m_behaviorTreeComponent->StartTree(*(m_behaviorTree));
	m_radius = AIEnemy->radius;
	m_selfPawn = InPawn;
}

void AEnemyController::SetEnemyKeyValue(APawn* player,float  radiusPos,float angleMinPos,float closeCounterRadius,float validAngleScalar,float radiusAttack)
{
	m_blackboardComponent->SetValueAsFloat("RadiusPos",radiusPos);
	m_blackboardComponent->SetValueAsObject("Player",Cast<AMyPlayer>(player));
	m_blackboardComponent->SetValueAsFloat("EnemyMinAngle",angleMinPos);
	m_blackboardComponent->SetValueAsFloat("CloseCounterRadius",closeCounterRadius);
	m_blackboardComponent->SetValueAsFloat("CloseCounterAngle",validAngleScalar);

	m_attackRadius = radiusAttack;
	m_radius = radiusPos;
	m_player = player;
}

bool AEnemyController::SetAttackState()
{
	if (CheckEnemiesDistancePlayer())
	{
		m_blackboardComponent->SetValueAsBool("IsAttacking",true);
		return true;
	}
	else
	{
		m_blackboardComponent->SetValueAsBool("IsAttacking",false);
		return false;
	}
}

bool AEnemyController::CheckEnemiesDistancePlayer()
{
	float distance = FVector::Dist(m_player->GetActorLocation(),m_selfPawn->GetActorLocation());
	if (distance < m_radius + m_attackRadius)
	{
		return  true;
	}
	else
	{
		return false;
	}
}

void AEnemyController::ResetAttackState()
{
	m_blackboardComponent->SetValueAsBool("IsAttacking",false);
}

void AEnemyController::EndBehaviorTree()
{
	m_behaviorTreeComponent->StopTree();
}

bool AEnemyController::GetAttackState()
{
	return m_blackboardComponent->GetValueAsBool("IsAttacking");
}

void AEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckObjestacle();
}


bool AEnemyController::CheckObjestacle()
{
	FHitResult hit;
	FVector start = m_player->GetActorLocation();
	FVector direction = (m_selfPawn->GetActorLocation() - m_player->GetActorLocation());
	direction.Z = 0.0f;
	direction.Normalize();
	FVector end = start + direction * (m_radius);
	FCollisionQueryParams traceParams;
	bool test = false;
	if (GetWorld()->LineTraceSingleByChannel(hit,start,end,ECC_Visibility,traceParams))
	{
		test = hit.Actor == this->GetPawn() ? true : false;
	}
	else
	{
		test = true;
	}
	return test;
}


bool AEnemyController::IsCloseEnoughToAttack()
{
	float distance = FVector::Dist(m_player->GetActorLocation(),m_selfPawn->GetActorLocation());
	if (distance <= m_attackRadius)
	{
		return true;
	}
	else
	{
		return false;
	}
}
