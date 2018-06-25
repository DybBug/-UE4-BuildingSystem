// Fill out your copyright notice in the Description page of Project Settings.

#include "Component_PlayerStats.h"
#include "../Widgets/Widget_Main.h"
#include "../Widgets/Widget_StatBar.h"
#include "../Characters/PlayerCharacter.h"

UComponent_PlayerStats::UComponent_PlayerStats()
: Super()
{
	m_Stats.Emplace(EStats::Shield, FStatValue(0, 0, 100));
	m_Stats.Emplace(EStats::Health, FStatValue(0, 100, 100));
}

void UComponent_PlayerStats::InitializeManager(APlayerCharacter* _pPlayer)
{
	m_pPlayer = _pPlayer;
	SetupStats();
}

UWidget_StatBar* UComponent_PlayerStats::GetBarForStat(EStats _Stat)
{
	if (m_pPlayer)
	{
		UWidget_Main* pMainWidget = m_pPlayer->GetMainWidget();

		if (_Stat == EStats::Shield)
		{
			return pMainWidget->GetShildBar();
		}
		else if (_Stat == EStats::Health)
		{
			return pMainWidget->GetHealthBar();
		}
	}

	return nullptr;
}

void UComponent_PlayerStats::_OnStatsSetup()
{
	TArray<EStats> Stats;
	m_Stats.GetKeys(Stats);
	for (auto& Iterator : Stats)
	{
		FStatValue StatValue;
		GetStat(Iterator, nullptr, &StatValue);
		GetBarForStat(Iterator)->InitializeBar(StatValue);
	}
}

void UComponent_PlayerStats::_UpdateStat(EStats _Stat)
{
	int CurrentValue;
	GetStat(_Stat, &CurrentValue, nullptr);
	GetBarForStat(_Stat)->UpdateStat(CurrentValue);
}
