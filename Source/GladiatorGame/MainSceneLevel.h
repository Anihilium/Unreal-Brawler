// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "WidgetUtils.h"
#include "MainSceneLevel.generated.h"

/**
 * 
 */
UCLASS()
class GLADIATORGAME_API AMainSceneLevel : public ALevelScriptActor
{
	GENERATED_BODY()

public :
	void CallVictory();
	void CallDefeat();
	void ChangeLevel();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Victory and Defeat")
		TSubclassOf<UUserWidget> m_winWidgetTemplate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Victory and Defeat")
		TSubclassOf<UUserWidget> m_defeatWidgetTemplate;
	UPROPERTY(EditAnywhere)
		FName MainMenuName;
protected:
	AMainSceneLevel();
	virtual void BeginPlay() override;

private :
	APlayerController* controller;
	UUserWidget* m_winWidget;
	UUserWidget* m_defWidget;
};
