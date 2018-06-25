// Fill out your copyright notice in the Description page of Project Settings.

#include "Component_MasterStatManager.h"


// Sets default values for this component's properties
UComponent_MasterStatManager::UComponent_MasterStatManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...	
}

void UComponent_MasterStatManager::SetupStats()
{
	TArray<EStats> Stats;
	m_Stats.GetKeys(Stats);

	for (auto& Iterator : Stats)
	{
		int CurrValue;
		GetStat(Iterator, &CurrValue, nullptr);
		SetStat(Iterator, CurrValue, false);
	}

	_OnStatsSetup();
}

void UComponent_MasterStatManager::ModifyStat(EStats _Stat, int _AddValue)
{
	if (_AddValue != 0)
	{
		int CurrValue;
		GetStat(_Stat, &CurrValue, nullptr);
		SetStat(_Stat, CurrValue + _AddValue, true);
	}
}

void UComponent_MasterStatManager::_OnStatsSetup()
{

}


void UComponent_MasterStatManager::_UpdateStat(EStats _Stat)
{

}


void UComponent_MasterStatManager::GetStat(EStats _Stat, int* out_pCurrentValue, FStatValue* out_pStatValue)
{
	FStatValue StatValue = m_Stats[_Stat];

	if (out_pCurrentValue != nullptr)
	{
		*out_pCurrentValue = StatValue.CurrentValue;
	}

	if (out_pStatValue != nullptr)
	{
		*out_pStatValue = StatValue;
	}
}

void UComponent_MasterStatManager::SetStat(EStats _Stat, int _NewValue, bool _bUpdate)
{
	TArray<EStats> Stats;
	m_Stats.GetKeys(Stats);

	if (Stats.Contains(_Stat))
	{
		FStatValue NewStatValue;
		GetStat(_Stat, nullptr, &NewStatValue);

		NewStatValue.CurrentValue = FMath::Clamp<int>(_NewValue, NewStatValue.MinValue, NewStatValue.MaxValue);

		m_Stats.Emplace(_Stat, NewStatValue);

		if (_bUpdate)
		{
			_UpdateStat(_Stat);
		}
	}
}



