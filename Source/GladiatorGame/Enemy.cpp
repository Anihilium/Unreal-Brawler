// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "EnemiesManager.h"
#include "EnemyController.h"

AEnemy::AEnemy()
{
	HammerCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOverlapBegin);
	HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBar->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	box = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxStrafeCollision"));
}
// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	PlayerBP = Cast<AMyPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn());
	radius = RadiusPositionningMax; 
	AEnemyController* controller = Cast<AEnemyController>(GetController());
	controller->SetEnemyKeyValue(PlayerBP,RadiusPositionningMax,AngleMinPosition,CloseCounterRadius,ValidAngleCloseCounter,RadiusToAttack);
	lifeBar = Cast<UHealthBar>(HealthBar->GetUserWidgetObject());
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!m_bIsDead)
	{
		FRotator newrot = (PlayerBP->GetActorLocation() - this->GetActorLocation()).Rotation();
		this->SetActorRotation(newrot.Quaternion());
	}

}

void AEnemy::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsAttacking || AlreadyDealtDamage)
		return;

	AMyPlayer* player = Cast<AMyPlayer, AActor>(OtherActor);
	if (player)
	{
		AlreadyDealtDamage = true;
		if (player->IsAttackBlocked(GetActorLocation()))
			return;
		player->MyTakeDamage(Damage);
	}
}

void AEnemy::DeadEvent()
{
	Super::DeadEvent();

	box->SetCollisionProfileName("NoCollision");
	AEnemyController* controller = Cast<AEnemyController>(this->GetController());
	controller->ResetAttackState();
	controller->EndBehaviorTree();
	Cast<AEnemiesManager>(manager)->RemoveEnemies(this);
}

void AEnemy::MyTakeDamage(const int damageAmount)
{
	if (Life < 1)
		return;

	Super::MyTakeDamage(damageAmount);

	if (Life < 1)
	{
		HealthBar->DestroyComponent();
		return;
	}

	lifeBar->HealthBar->SetPercent(Life * 1.0f / MaxLife * 1.0f);
}

TArray<float> AEnemy::GetAngleArray()
{
	return Cast<AEnemiesManager>(manager)->enemiesAnglePos;
}