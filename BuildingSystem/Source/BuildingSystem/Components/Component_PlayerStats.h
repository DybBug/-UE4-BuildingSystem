// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Component_MasterStatManager.h"
#include "Component_PlayerStats.generated.h"

class UWidget_StatBar;
class APlayerCharacter;
/**
 * 
 */
UCLASS()
class BUILDINGSYSTEM_API UComponent_PlayerStats : public UComponent_MasterStatManager
{
	GENERATED_BODY()
	
public :
	UComponent_PlayerStats();
	
	void InitializeManager(APlayerCharacter* _pPlayer);

	// #. �ش� ������ ���α׷������� ���۷����� �����´�.
	UWidget_StatBar* GetBarForStat(EStats _Stat);

private :
	virtual void _OnStatsSetup() override;
	virtual void _UpdateStat(EStats _Stat) override;
protected :
	APlayerCharacter* m_pPlayer;
};
