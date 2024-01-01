// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AEntity::AEntity()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(radiusSphere, 60.0f);

	HammerSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hammer"));
	HammerSkeletalMesh->SetupAttachment(GetMesh(), TEXT("WeaponPoint"));

	HammerCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("HammerCollision"));
	HammerCollision->SetupAttachment(HammerSkeletalMesh, TEXT("ColliderSocket"));
	HammerCollision->SetBoxExtent(FVector(12.f, 12.f, 19.f));

	IsAttacking = false;
	AlreadyDealtDamage = false;

}

// Called when the game starts or when spawned
void AEntity::BeginPlay()
{
	Super::BeginPlay();
	Life = MaxLife;
	DwarfMaterial = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(0), this);
	GetMesh()->SetMaterial(0, DwarfMaterial);

}

void AEntity::MyTakeDamage(const int DamageAmount)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodPS, GetActorLocation());
	Life -= DamageAmount;
	DwarfMaterial->SetVectorParameterValue("Tint", FLinearColor(1, 0, 0, 1));
	FTimerHandle _;
	GetWorldTimerManager().SetTimer(_, this, &AEntity::GetColorBack, 0.5f, false);
	if (Life == 0)
	{
		DeadEvent();
	}
}
void AEntity::DeadEvent()
{
	HammerCollision->SetCollisionProfileName("NoCollision");
	GetCapsuleComponent()->SetCollisionProfileName("NoCollision");
	m_bIsDead = true;
}

void AEntity::GetColorBack()
{
	DwarfMaterial->SetVectorParameterValue("Tint", FLinearColor(1, 1, 1, 1));
}

void AEntity::Attack()
{
	if (IsAttacking)
		return;

	IsAttacking = true;
	float duration = GetMesh()->AnimScriptInstance->Montage_Play(AttackMontage);
	FTimerHandle _;
	GetWorldTimerManager().SetTimer(_, this, &AEntity::AttackEnd, duration, false);
}

void AEntity::AttackEnd()
{
	IsAttacking = false;
	AlreadyDealtDamage = false;
}

// Called every frame
void AEntity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEntity::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

