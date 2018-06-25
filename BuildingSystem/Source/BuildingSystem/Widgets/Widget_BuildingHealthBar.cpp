// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget_BuildingHealthBar.h"
#include "../BuildingSystemEnums.h"

#include <Components/ProgressBar.h>
#include <Components/TextBlock.h>
#include <Kismet/KismetTextLibrary.h>

#define LOCTEXT_NAMESPACE "Format"


void UWidget_BuildingHealthBar::InitializeWidget(int _MaxHealth)
{
	m_MaxHealth = _MaxHealth;
	SetState(EBuildingStates::Building);
	UpdateHealth(m_MaxHealth);
}

void UWidget_BuildingHealthBar::UpdateHealth(int _NewValue)
{
	m_CurrentHealth = FMath::Clamp<int>(_NewValue, 0, m_MaxHealth);

	float Percent = (float)m_CurrentHealth / (float)m_MaxHealth;
	m_pHealthBar->SetPercent(Percent);

	FText TextFormat = FText::Format(LOCTEXT("Format", "{0} | {1}"), m_CurrentHealth, m_MaxHealth);
	m_pHealthText->SetText(TextFormat);
}

void UWidget_BuildingHealthBar::SetState(EBuildingStates _NewState)
{
	m_State = _NewState;
	
	FLinearColor Color = (m_State == EBuildingStates::Building) ? FColor(0xFF864803) : FColor(0xFF23A518);	
	m_pHealthBar->SetFillColorAndOpacity(Color);
}
