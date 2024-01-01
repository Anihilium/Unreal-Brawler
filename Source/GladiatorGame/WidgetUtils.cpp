// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetUtils.h"
#include "Blueprint/WidgetBlueprintLibrary.h"


UUserWidget* UWidgetUtils::CreateAndShowWidget(TSubclassOf<UUserWidget> widgetFocus, APlayerController* controller, TEnumAsByte<InputModeGame> inputModeState, bool bShowMouse, bool bAddViewport)
{
	UUserWidget* widgetCreate = CreateWidget(controller, widgetFocus);
	if (bAddViewport) widgetCreate->AddToViewport();
	else widgetCreate->RemoveFromViewport();

	controller->bShowMouseCursor = bShowMouse;
	SetInputMode(inputModeState, controller, widgetCreate);
	return widgetCreate;
}

void UWidgetUtils::ShowWidget(UUserWidget* widgetFocus, APlayerController* controller, TEnumAsByte<InputModeGame> inputModeState, bool bShowMouse , bool bAddViewport)
{
	if (bAddViewport) widgetFocus->AddToViewport();
	else widgetFocus->RemoveFromViewport();

	controller->bShowMouseCursor = bShowMouse;
	SetInputMode(inputModeState, controller,widgetFocus);

}


void UWidgetUtils::SetInputMode(TEnumAsByte<InputModeGame> state, APlayerController* controller, UUserWidget* widget)
{
	switch (state)
	{
	case InputModeGame::UIOnly:
		UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(controller, widget);
		break;
	case InputModeGame::Game:
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(controller);
		break;
	case InputModeGame::Both:
		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(controller, widget);
		break;
	default:
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(controller);
		break;
	}
}
