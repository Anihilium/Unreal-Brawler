// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemiesManager.h"
#include "DrawDebugHelpers.h"
#include "Blueprint/WidgetTree.h"
#include "Kismet/GameplayStatics.h"
#include "MainSceneLevel.h"
#include "MyPlayer.h"


// Sets default values
AEnemiesManager::AEnemiesManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemiesManager::BeginPlay()
{
	Super::BeginPlay();
	SetUIFeedback();

}

void AEnemiesManager::SetUIFeedback()
{
	playerController = GetWorld()->GetFirstPlayerController();
	m_uiFeedback = CreateWidget(playerController,UiWaveFeedback);
	m_waveText = (UTextBlock*)m_uiFeedback->WidgetTree->FindWidget(uiNameText);
}


// Called every frame
void AEnemiesManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	TimerAttacksSelect(DeltaTime);

	if (!m_isFinishAllWave)
	{
		FName value = m_uiFeedback->GetFName();
		if (value == "None")
		{
			m_IsBetweenWave = !m_IsBetweenWave;
			m_IsBetweenWave = !m_IsBetweenWave;
		}
		if (m_IsBetweenWave)
		{
			RunTimerBetweenWave(DeltaTime);
		}

		if (!m_IsBetweenWave && AllEnemiesAreDead())
		{

			m_IsBetweenWave = true;
			EndWaveFeedback(true);
			Cast<AMyPlayer>(playerController->GetPawn())->SelfHeal();
			if (m_waveCurrentNumber == waveManagement.Num())
			{
				m_isFinishAllWave = true;
				Cast<AMainSceneLevel>(GetWorld()->GetLevelScriptActor())->CallVictory();
			}
		}
		
	}
	else
	{

		
	}
}

void AEnemiesManager::SpawnEnemies(int number,bool hasBoss)
{

	int count = number;
	if (hasBoss)count++;

	TSubclassOf<AEnemy> entityToSpawn = ActorToSpawn;

	for (int i = 0; i < count; i++)
	{
		if (i == number && hasBoss) entityToSpawn = BossToSpawn;

		FActorSpawnParameters spawnInfo = FActorSpawnParameters();
		spawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		FRotator myRot(0,0,0);
		FVector loc = FVector(FMath::RandRange(radius / 2.0f, radius) * FMath::RandRange(-1, 1), FMath::RandRange(radius / 2.0f, radius) * FMath::RandRange(-1, 1),0.0f);
		loc = center + loc ;

		AEnemy* enemy = GetWorld()->SpawnActor<AEnemy>(entityToSpawn,loc,myRot,spawnInfo);
		m_enemiesGroup.Add(enemy);
		m_enemiesGroup.Last()->manager = this;
		m_enemiesGroup.Last()->index = i;
	}
	enemiesAnglePos.Empty();
	enemiesAnglePos.SetNum(count);
	

}

void AEnemiesManager::RemoveEnemies(AEnemy* target)
{
	if (m_enemiesGroup.Contains(target))
	{
		m_enemiesGroup.Remove(target);
		enemiesAnglePos[target->index] = 1000;
	}
}

void AEnemiesManager::SelectEnemyToAttack()
{
	if (m_enemiesGroup.Num() == 0) return;

	int index = (int)FMath::FRandRange(0,m_enemiesGroup.Num());
	controller = Cast<AEnemyController>(m_enemiesGroup[index]->GetController());
	if (!controller->SetAttackState()  && indexAttack <6)
	{
		indexAttack++;
		SelectEnemyToAttack();
	}
	indexAttack = 0;

}

void AEnemiesManager::TimerAttacksSelect(float deltaTime)
{
	if (CanSelectAttack())
	{

		if (m_attackTimer > TimeBetweenAttack)
		{
			SelectEnemyToAttack();
			m_attackTimer = 0.0f;
		}
		else
		{
			m_attackTimer += deltaTime;
		}
	}
	else 
	{
		controller->SetAttackState();

	}

}

bool AEnemiesManager::CanSelectAttack()
{
	if (controller)
	{
		return !(controller->GetAttackState());
	}
	else
	{
		return true;
	}
}

#pragma region WaveManagement

void AEnemiesManager::RunTimerBetweenWave(float deltaTime)
{

	if (m_waveTimer > TimeBetweenWave)
	{

		LaunchWave();
		m_waveTimer = 0.0f;
		m_HasLaunchFeedback = false;;
		m_IsBetweenWave = false;
		StartWaveFeedback(false);
	}
	else
	{
		if (TimeBetweenWave - m_waveTimer <= 2.0f && !m_HasLaunchFeedback)
		{

			EndWaveFeedback(false);
			StartWaveFeedback(true);

			m_HasLaunchFeedback = true;
		}
		if (m_HasLaunchFeedback)
		{
			FText textUI = FText::FromString(FString("New wave will start in ") + FString::Printf(TEXT("%.0f"),TimeBetweenWave - m_waveTimer));
			m_waveText->SetText(textUI);
		}
		m_waveTimer += deltaTime;
	}
}
void AEnemiesManager::StartWaveFeedback(bool showWidget)
{
	UWidgetUtils::ShowWidget(m_uiFeedback,playerController,InputModeGame::Game,false,showWidget);
}


void AEnemiesManager::LaunchWave()
{
	SpawnEnemies(waveManagement[m_waveCurrentNumber].enemiesCount,waveManagement[m_waveCurrentNumber].hasBoss);
	m_waveCurrentNumber++;
}
void AEnemiesManager::EndWaveFeedback(bool showWidget)
{
	UWidgetUtils::ShowWidget(m_uiFeedback,playerController,InputModeGame::Game,false,showWidget);
	FText textUI = FText::FromString(FString("Wave Finish "));
	m_waveText->SetText(textUI);
}

#pragma endregion

