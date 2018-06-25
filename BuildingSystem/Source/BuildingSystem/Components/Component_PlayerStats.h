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

	// #. 해당 스텟의 프로그래스바의 레퍼런스를 갖져온다.
	UWidget_StatBar* GetBarForStat(EStats _Stat);

private :
	virtual void _OnStatsSetup() override;
	virtual void _UpdateStat(EStats _Stat) override;
protected :
	APlayerCharacter* m_pPlayer;
};
