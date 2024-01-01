// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entity.h"
#include "Menu.h"
#include "PlayerHUD.h"
#include "MyPlayer.generated.h"


/**
*
*/
UCLASS()
class GLADIATORGAME_API AMyPlayer : public AEntity
{
	GENERATED_BODY()

public:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = Camera,meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = Camera,meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
		USkeletalMeshComponent* ShieldSkeletalMesh;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = Camera)
		float BaseLookUpRate;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp,class AActor* OtherActor,class UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere)
		float BlockSemiAngle;

	UPROPERTY(EditAnywhere)
		int LowLifeThreshold;

	bool IsBlocking;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
		TSubclassOf<UMenu> PauseMenu;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
		TSubclassOf<UPlayerHUD> UserHUD;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<UMenu> DefeatHUD;

protected:
	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	void BeginPlay() override;

	void Attack();
	void Block();
	void BlockEnd();
	void PauseGame();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	AMyPlayer();
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	bool IsAttackBlocked(FVector enemyPos);
	void MyTakeDamage(const int damageAmount) override;
	void SelfHeal();
	UPROPERTY(EditAnywhere)
		UMaterial* VignetteMaterial;
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere)
		float IncrTimeVignette;
	UPROPERTY(VisibleInstanceOnly,Category = Runtime)
		UUserWidget* mPauseMenuWidget = nullptr;
	UPROPERTY(VisibleInstanceOnly, Category = Runtime)
		UUserWidget* mPlayerHUD = nullptr;
	UPROPERTY(VisibleInstanceOnly, Category = Runtime)
		UUserWidget* mDefeatHUD = nullptr;

private:
	UPlayerHUD* mPlayerHUDInstance;
	UProgressBar* mPlayerHealthBar;
	APlayerController* mPlayerController;
	UWorld* mWorld;
	bool IsGamePaused;
};
