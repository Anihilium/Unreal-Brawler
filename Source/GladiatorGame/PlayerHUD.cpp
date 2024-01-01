// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"

void UPlayerHUD::NativeConstruct()
{
	Super::NativeConstruct();
}

//void  UPlayerHUD::UpdateLifeBar(float ratio)
//{
//	HealthBar->Percent = ratio;
//	if (ratio > 0.25)
//	{
//		if (!LowHP)
//		{
//			LowHP = true;
//			LowHealth();
//		}
//	}
//	else
//	{
//		if (LowHP)
//		{
//			LowHP = false;
//			HighHealth();
//		}
//	}
//}
