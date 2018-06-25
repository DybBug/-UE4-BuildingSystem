// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Component_MasterStatManager.h"
#include "Component_BuildingStats.generated.h"

class ABuilding_Base;
/**
 * 
 */
UCLASS()
class BUILDINGSYSTEM_API UComponent_BuildingStats : public UComponent_MasterStatManager
{
	GENERATED_BODY()
	
public :
	UComponent_BuildingStats();

public :
	void InitializeManager(ABuilding_Base* _pBuilding);

	int GetMaxHealth();
	
	void SetMaxStatValue(EStats _Stat, int _NewValue, bool _bUpdate);

protected :
	virtual void _UpdateStat(EStats _Stat) override;


protected :
	ABuilding_Base* m_pBuilding;
	
};
