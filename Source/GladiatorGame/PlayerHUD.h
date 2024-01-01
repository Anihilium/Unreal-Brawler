// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Camera/CameraComponent.h"
#include "PlayerHUD.generated.h"

/**
 *
 */
UCLASS()
class GLADIATORGAME_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* HealthBar;
	UPROPERTY(BlueprintReadOnly)
		bool LowHP = false;

	

	UMaterialInstanceDynamic* VignetteMaterialIntance;


	//void SetupHUD(UCameraComponent* cam);
	//void LowHealth();
	//void HighHealth();
protected:
	//UFUNCTION(BlueprintCallable)
	//	void UpdateLifeBar(float ratio);
	void NativeConstruct() override;
};
