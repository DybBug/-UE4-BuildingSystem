// Fill out your copyright notice in the Description page of Project Settings.

#include "Component_BuildingStats.h"
#include "../Buildings/Building_Base.h"
#include "../Widgets/Widget_BuildingHealthBar.h"


UComponent_BuildingStats::UComponent_BuildingStats()
: Super()
{
	m_Stats.Emplace(EStats::Health, FStatValue(0, 0, 100));
}

void UComponent_BuildingStats::InitializeManager(ABuilding_Base* _pBuilding)
{
	if (_pBuilding)
	{
		m_pBuilding = _pBuilding;
		SetupStats();
	}
}

int UComponent_BuildingStats::GetMaxHealth()
{
	FStatValue Value;
	GetStat(EStats::Health, nullptr, &Value);

	return Value.MaxValue;
}

void UComponent_BuildingStats::SetMaxStatValue(EStats _Stat, int _NewValue, bool _bUpdate)
{
	TArray<EStats> Keys;
	m_Stats.GetKeys(Keys);

	if (Keys.Contains(_Stat))
	{
		FStatValue Value;
		int CurrentValue;
		GetStat(_Stat, &CurrentValue, &Value);

		CurrentValue = FMath::Clamp<int>(CurrentValue, Value.CurrentValue, _NewValue);

		m_Stats.Emplace(_Stat, FStatValue(Value.MaxValue, CurrentValue, _NewValue));

		if (_bUpdate)
		{
			_UpdateStat(_Stat);
		}
	}	
}

void UComponent_BuildingStats::_UpdateStat(EStats _Stat)
{
	if ((_Stat == EStats::Health) && m_pBuilding->GetInPlayerSight())
	{
		int CurrentValue;
		GetStat(EStats::Health, &CurrentValue, nullptr);
		m_pBuilding->GetHealthBarWidget_Ref()->UpdateHealth(CurrentValue);
	}
}


