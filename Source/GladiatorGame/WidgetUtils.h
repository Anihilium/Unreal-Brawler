// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetUtils.generated.h"

UENUM()
enum InputModeGame
{
	UIOnly     UMETA(DisplayName = "UI"),
	Game      UMETA(DisplayName = "Game"),
	Both   UMETA(DisplayName = "Both"),
};

UCLASS()
class GLADIATORGAME_API UWidgetUtils : public UUserWidget
{
	GENERATED_BODY()
public:
	static UUserWidget* CreateAndShowWidget(TSubclassOf<UUserWidget> widgetFocus, APlayerController* controller, TEnumAsByte<InputModeGame> inputModeState, bool bShowMouse = true, bool bAddViewport = true);
	static void ShowWidget(UUserWidget* widgetFocus, APlayerController* controller, TEnumAsByte<InputModeGame> inputModeState, bool bShowMouse = true, bool bAddViewport = true);

private:
	static void SetInputMode(TEnumAsByte<InputModeGame> state, APlayerController* controller, UUserWidget* widget);
};

