// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../BuildingSystemEnums.h"
#include "../BuildingSystemStructure.h"
#include "Component_MasterStatManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGSYSTEM_API UComponent_MasterStatManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UComponent_MasterStatManager();

public :
	void SetupStats();
	void ModifyStat(EStats _Stat, int _AddValue);

	void GetStat(EStats _Stat, int* out_pCurrentValue, FStatValue* out_pStatValue);
	void SetStat(EStats _Stat, int _NewValue, bool _bUpdate = true);

protected :
	virtual void _OnStatsSetup();
	virtual void _UpdateStat(EStats _Stat);



protected :
	UPROPERTY(EditAnywhere)
	TMap<EStats, FStatValue> m_Stats;

};
