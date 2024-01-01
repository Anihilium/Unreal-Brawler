// Fill out your copyright notice in the Description page of Project Settings.


#include "MainSceneLevel.h"
#include "Kismet/GameplayStatics.h"

AMainSceneLevel::AMainSceneLevel()
{

}

void AMainSceneLevel::CallVictory()
{
	m_winWidget = UWidgetUtils::CreateAndShowWidget(m_winWidgetTemplate, controller, InputModeGame::UIOnly);
}

void AMainSceneLevel::ChangeLevel()
{
	UGameplayStatics::OpenLevel(GetGameInstance(), MainMenuName);
}

void AMainSceneLevel::CallDefeat()
{
	m_defWidget = UWidgetUtils::CreateAndShowWidget(m_defeatWidgetTemplate, controller, InputModeGame::UIOnly);
}

void AMainSceneLevel::BeginPlay()
{
	Super::BeginPlay();
	controller = GetWorld()->GetFirstPlayerController();

}