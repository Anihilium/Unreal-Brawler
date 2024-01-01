// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "Blueprint/UserWidget.h"
#include "WidgetUtils.h"
#include "LevelSetupActor.generated.h"



UCLASS()
class GLADIATORGAME_API ALevelSetupActor : public ALevelScriptActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup")
		TSubclassOf<UUserWidget> m_widgetTemplate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup")
		TEnumAsByte<InputModeGame> StartInputMode;
protected:
	ALevelSetupActor();
	virtual void BeginPlay() override;

private:
	UUserWidget* m_mainMenuWidget;
	APlayerController* m_playerController;
};

