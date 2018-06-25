// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget_StatBar.h"

#include <Components/Image.h>
#include <Components/ProgressBar.h>
#include <Components/TextBlock.h>


void UWidget_StatBar::NativePreConstruct()
{
	UUserWidget::NativePreConstruct();

	m_pIcon->SetBrushFromTexture(m_pStatIcon, true);
	m_pBar->SetFillColorAndOpacity(m_BarColor);
}


void UWidget_StatBar::InitializeBar(const FStatValue& _StatValue)
{
	m_MinValue     = _StatValue.MinValue;
	m_CurrentValue = _StatValue.CurrentValue;
	m_MaxValue     = _StatValue.MaxValue;
	m_bInitialized = true;

	m_pMaxText->SetText(FText::AsNumber(m_MaxValue));
	UpdateStat(m_CurrentValue);
}

void UWidget_StatBar::UpdateStat(int _NewValue)
{
	if (m_bInitialized)
	{
		m_CurrentValue = _NewValue;

		m_pCurrentText->SetText(FText::AsNumber(m_CurrentValue));

		int   CurrentValue = m_CurrentValue - m_MinValue;
		int   MaxValue     = m_MaxValue - m_MinValue;
		float Percent      = (float)CurrentValue / (float)MaxValue;

		m_pBar->SetPercent(Percent);
	}
}


