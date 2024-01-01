// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayer.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "WidgetUtils.h"
#include "Enemy.h"

//////////////////////////////////////////////////////////////////////////
// AMyPlayer

AMyPlayer::AMyPlayer()
{
	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	IsBlocking = false;
	IsGamePaused = false;

	HammerCollision->OnComponentBeginOverlap.AddDynamic(this, &AMyPlayer::OnOverlapBegin);

	ShieldSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Shield"));
	ShieldSkeletalMesh->SetupAttachment(GetMesh(), TEXT("DualWeaponPoint"));
}

void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();
	mWorld = GetWorld();
	mPlayerController = mWorld->GetFirstPlayerController();
	mDefeatHUD = CreateWidget(mPlayerController, DefeatHUD);
	mPauseMenuWidget = CreateWidget(mPlayerController, PauseMenu);
	mPlayerHUD = CreateWidget(mPlayerController, UserHUD);
	mPlayerHUD->AddToViewport();
	mPlayerHUDInstance = Cast<UPlayerHUD, UUserWidget>(mPlayerHUD);
	mPlayerHealthBar = mPlayerHUDInstance->HealthBar;
	mPlayerHUDInstance->VignetteMaterialIntance = UMaterialInstanceDynamic::Create(VignetteMaterial, this);
	FollowCamera->AddOrUpdateBlendable(mPlayerHUDInstance->VignetteMaterialIntance, 1.f);
	//mPlayerHUDInstance->SetupHUD(FollowCamera);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMyPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMyPlayer::Attack);
	PlayerInputComponent->BindAction("Block", IE_Pressed, this, &AMyPlayer::Block);
	PlayerInputComponent->BindAction("Block", IE_Released, this, &AMyPlayer::BlockEnd);

	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &AMyPlayer::PauseGame).bExecuteWhenPaused = true;

	PlayerInputComponent->BindAxis("HorizontalCameraMovement", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("VerticalCameraMovement", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMyPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyPlayer::MoveRight);

	PlayerInputComponent->BindAxis("TurnRate", this, &AMyPlayer::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMyPlayer::LookUpAtRate);
}

void AMyPlayer::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * mWorld->GetDeltaSeconds());
}

void AMyPlayer::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * mWorld->GetDeltaSeconds());
}

void AMyPlayer::MoveForward(float Value)
{
	if (IsBlocking || IsAttacking)
		return;

	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMyPlayer::MoveRight(float Value)
{
	if (IsBlocking || IsAttacking)
		return;

	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

bool AMyPlayer::IsAttackBlocked(FVector enemyPos)
{
	if (!IsBlocking) return false;
	FVector playerToEnemyNormalized = enemyPos - GetActorLocation();
	playerToEnemyNormalized.Normalize();
	return FMath::Acos(FVector::DotProduct(GetActorForwardVector(), playerToEnemyNormalized)) < FMath::DegreesToRadians(BlockSemiAngle);
}

void AMyPlayer::Attack()
{
	if (IsBlocking)
		return;

	Super::Attack();
}

void AMyPlayer::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsAttacking || AlreadyDealtDamage)
		return;

	AEnemy* enemy = Cast<AEnemy, AActor>(OtherActor);
	if (enemy)
	{
		enemy->MyTakeDamage(Damage);
		AlreadyDealtDamage = true;
	}
}

void AMyPlayer::Block()
{
	if (IsBlocking || IsAttacking)
		return;

	IsBlocking = true;
	ShieldSkeletalMesh->SetWorldScale3D(FVector(2.f, 2.f, 2.f));
}

void AMyPlayer::BlockEnd()
{
	ShieldSkeletalMesh->SetWorldScale3D(FVector(1.f, 1.f, 1.f));
	IsBlocking = false;
}

void AMyPlayer::MyTakeDamage(const int damageAmount)
{

	Super::MyTakeDamage(damageAmount);

	if (Life < 1)
	{
		UWidgetUtils::ShowWidget(mDefeatHUD, mPlayerController, InputModeGame::Both, true, true);
		UGameplayStatics::SetGamePaused(mWorld, true);
		return;
	}

	mPlayerHealthBar->SetPercent(Life * 1.0f / MaxLife * 1.0f);
}

void AMyPlayer::SelfHeal()
{
	if (Life == MaxLife)
		return;

	if (Life < LowLifeThreshold)
		Life = LowLifeThreshold;

	Life++;

	mPlayerHealthBar->SetPercent(Life * 1.0f / MaxLife * 1.0f);
	mPlayerHUDInstance->VignetteMaterialIntance->SetScalarParameterValue("Value", 1.f);
}

void AMyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Life <= LowLifeThreshold)
	{
		static float timeParam = 0.f;
		timeParam += Life > 1 ? IncrTimeVignette : IncrTimeVignette * 2.f;

		mPlayerHUDInstance->VignetteMaterialIntance->SetScalarParameterValue("Value", FMath::Sin(timeParam) * 0.5f + 0.5f);
	}
}

void AMyPlayer::PauseGame()
{
	if (Life < 1)
		return;

	if (!IsGamePaused)
		UWidgetUtils::ShowWidget(mPauseMenuWidget, mPlayerController, InputModeGame::Both, true, true);
	else
		UWidgetUtils::ShowWidget(mPauseMenuWidget, mPlayerController, InputModeGame::Game, false, false);

	UGameplayStatics::SetGamePaused(mWorld, !IsGamePaused);
	IsGamePaused = !IsGamePaused;
}