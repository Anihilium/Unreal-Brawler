// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelSetupActor.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

ALevelSetupActor::ALevelSetupActor()
{

}

void ALevelSetupActor::BeginPlay()
{
	Super::BeginPlay();
	m_playerController = GetWorld()->GetFirstPlayerController();
	if (m_widgetTemplate != NULL)
	{
		m_mainMenuWidget = CreateWidget(m_playerController, m_widgetTemplate);
	}
	UWidgetUtils::ShowWidget(m_mainMenuWidget,m_playerController, StartInputMode, true, true);

}

