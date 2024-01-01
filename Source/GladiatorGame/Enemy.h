// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "MyPlayer.h"
#include "Components/WidgetComponent.h"
#include "HealthBar.h"
#include "Entity.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/BoxComponent.h"
#include "Enemy.generated.h"

/**
 * 
 */
UCLASS()
class GLADIATORGAME_API AEnemy : public AEntity
{
	GENERATED_BODY()

public:
	AEnemy();
	UPROPERTY(EditAnywhere,Category = AI)
		UBehaviorTree* m_behaviorTree;
	UPROPERTY(EditAnywhere,Category = AI)
		AMyPlayer* PlayerBP;
	UPROPERTY(EditAnywhere, Category = Positioning)
		float RadiusPositionningMin = 200;
	UPROPERTY(EditAnywhere, Category = Positioning)
		float RadiusPositionningMax = 300;
	UPROPERTY(EditAnywhere,Category = Positioning)
		float AngleMinPosition = 25;
	UPROPERTY(EditAnywhere,Category = Positioning)
		float CloseCounterRadius = 100;
	UPROPERTY(EditAnywhere,Category = Positioning)
		float ValidAngleCloseCounter = .45f;
	UPROPERTY(EditAnywhere,Category = Positioning)
		float RadiusToAttack = 200.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UWidgetComponent* HealthBar;
	
	AActor* manager;
	float radius;
	int index;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void DeadEvent() override;
	void MyTakeDamage(const int damageAmount) override;
	  TArray<float> GetAngleArray();

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void  Tick(float DeltaTime) override;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UBoxComponent* box;

private:
	UHealthBar* lifeBar;
};
