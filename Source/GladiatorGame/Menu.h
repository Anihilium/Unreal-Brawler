// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include <Components/Button.h>
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Engine/World.h"
#include "UObject/SoftObjectPtr.h"
#include "Menu.generated.h"

UCLASS()
class GLADIATORGAME_API UMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable)
		void ChangeLevel(TSoftObjectPtr<UWorld> level);
	UFUNCTION(BlueprintCallable)
		void QuitApp();
	UFUNCTION(BlueprintCallable)
		void CloseWidget();
	UFUNCTION(BlueprintCallable)
		void ResumeGame();
	
	void NativeConstruct() override;


private:
	APawn* m_playerPawn;
	APlayerController* m_playerController;
};
