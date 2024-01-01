// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "Entity.generated.h"

/**
 *
 */
UCLASS()
class GLADIATORGAME_API AEntity : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEntity();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		int MaxLife;

	UPROPERTY(VisibleAnywhere)
		int Life;

	UPROPERTY(EditAnywhere)
		int Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		USkeletalMeshComponent* HammerSkeletalMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UBoxComponent* HammerCollision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UParticleSystem* BloodPS;


	float radiusSphere = 40.0f;
	
	virtual void MyTakeDamage(const int damageAmount);
	virtual void DeadEvent();
	void Attack();
	void AttackEnd();
	inline bool IsDead() { return m_bIsDead; }

	bool IsAttacking, AlreadyDealtDamage;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UMaterialInstanceDynamic* DwarfMaterial;

	void GetColorBack();
	bool m_bIsDead;
private:
};
