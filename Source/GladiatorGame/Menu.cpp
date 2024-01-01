// Fill out your copyright notice in the Description page of Project Settings.

#include "Menu.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"



void UMenu::NativeConstruct()
{
	Super::NativeConstruct();
	m_playerPawn = GetOwningPlayerPawn();
	m_playerController = GetOwningPlayer();
}


void UMenu::ChangeLevel(TSoftObjectPtr<UWorld> level)
{
	RemoveFromViewport();
	m_playerController->SetShowMouseCursor(false);
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(m_playerController);
	UGameplayStatics::OpenLevelBySoftObjectPtr(m_playerPawn, level);
}
void UMenu::QuitApp()
{
	UKismetSystemLibrary::QuitGame(m_playerPawn, m_playerController, EQuitPreference::Quit, true);
}

void UMenu::ResumeGame()
{
	RemoveFromViewport();
	m_playerController->SetShowMouseCursor(false);
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(m_playerController);
	m_playerController->SetPause(false);
}

void UMenu::CloseWidget()
{
	RemoveFromViewport();
}