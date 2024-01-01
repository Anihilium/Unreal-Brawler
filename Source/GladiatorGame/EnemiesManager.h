// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.h"
#include "MyPlayer.h"
#include "EnemyController.h"
#include "Components/TextBlock.h"
#include "WidgetUtils.h"
#include "EnemiesManager.generated.h"

USTRUCT()
struct FWave
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere,Category = Spawn)
		int enemiesCount = 0;
	UPROPERTY(EditAnywhere,Category = Spawn)
		bool hasBoss = false;

	FWave() : enemiesCount(0),hasBoss(false)
	{
	}
};


UCLASS()
class GLADIATORGAME_API AEnemiesManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemiesManager();
	UPROPERTY(EditAnywhere,Category = Spawn)
		int enemyNumber = 5;
	UPROPERTY(EditAnywhere,Category = Spawn)
		FVector center;
	UPROPERTY(EditAnywhere,Category = Spawn)
		float radius;
	UPROPERTY(EditAnywhere,Category = Attacks)
		float TimeBetweenAttack = 1.f;
	UPROPERTY(EditAnywhere,meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AEnemy> ActorToSpawn;
	UPROPERTY(EditAnywhere,meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AEnemy> BossToSpawn;

	UPROPERTY(EditAnywhere,Category = Waves)
		float TimeBetweenWave = 5.0f;
	UPROPERTY(EditAnywhere,Category = Waves)
		TSubclassOf<UUserWidget> UiWaveFeedback;
	UPROPERTY(EditAnywhere,Category = Waves)
		FName uiNameText;
	UPROPERTY(EditAnywhere,Category = Waves)
		TArray<FWave> waveManagement;
	UPROPERTY(EditAnywhere,Category = Waves)
		UUserWidget* m_uiFeedback = nullptr;

	TArray<float> enemiesAnglePos;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void SetUIFeedback();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SpawnEnemies(int number,bool hasBoss);
	void RemoveEnemies(AEnemy* targer);
	inline  TArray<AEnemy*> GetEnemies() { return  m_enemiesGroup; }
private:
	TArray<AEnemy*> m_enemiesGroup = TArray<AEnemy*>();
	APlayerController* playerController = nullptr;
	AEnemyController* controller = nullptr;
	UTextBlock* m_waveText = nullptr;

	float m_attackTimer;
	int m_waveCurrentNumber;
	float m_waveTimer;
	bool m_IsBetweenWave = true;
	bool m_HasLaunchFeedback;
	bool m_HasEndWave;
	bool m_isFinishAllWave = false;

	void SelectEnemyToAttack();
	void TimerAttacksSelect(float deltaTime);
	void RunTimerBetweenWave(float deltaTime);
	void StartWaveFeedback(bool showWidget);
	void LaunchWave();
	void EndWaveFeedback(bool showWidget);
	inline	bool AllEnemiesAreDead() { return m_enemiesGroup.Num() == 0 ? true : false; }
	bool CanSelectAttack();
	int indexAttack = 5;

};
