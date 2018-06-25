// Fill out your copyright notice in the Description page of Project Settings.

#include "Component_MasterResourceManager.h"
#include "../Resources/Resource_Base.h"


// Sets default values for this component's properties
UComponent_MasterResourceManager::UComponent_MasterResourceManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UComponent_MasterResourceManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UComponent_MasterResourceManager::AddResource(TSubclassOf<AResource_Base> _ResourceClass, int _AddAmount)
{
	if (_ResourceClass.Get() && (_AddAmount > 0))
	{
		int* Amount = m_Resources.Find(_ResourceClass);

		// #. 자원의 수량이 0이 아니면.
		if (Amount)
		{
			*Amount += _AddAmount;
			*Amount = FMath::Clamp<int>(*Amount, 0, m_MaxAmount);
			m_Resources.Emplace(_ResourceClass, *Amount);			
		}
		// #. 자원의 수량이 0이면 새로 추가 한다.
		else
		{
			_AddAmount = FMath::Clamp<int>(_AddAmount, 0, m_MaxAmount);
			m_Resources.Emplace(_ResourceClass, _AddAmount);			
		}

		// 추가된 수량만큼 화면의 자원 업데이트.
		_UpdateResourceDisplay(_ResourceClass);
	}
}

void UComponent_MasterResourceManager::RemoveResource(TSubclassOf<AResource_Base> _ResourceClass, int _RemoveAmount)
{
	if (_ResourceClass.Get() && (_RemoveAmount > 0))
	{
		int* Amount = m_Resources.Find(_ResourceClass);

		// #. 자원의 수량이 0이 아니면.
		if (Amount)
		{
			*Amount -= _RemoveAmount;
			*Amount = FMath::Clamp<int>(*Amount, 0, m_MaxAmount);
			m_Resources.Emplace(_ResourceClass, *Amount);		

			// 추가된 수량만큼 화면의 자원 업데이트.
			_UpdateResourceDisplay(_ResourceClass);
		}
	}
}

int UComponent_MasterResourceManager::GetResourceAmount(TSubclassOf<AResource_Base> _ResourceClass)
{
	int* pAmount = m_Resources.Find(_ResourceClass);
	if (pAmount)
	{
		return *pAmount;
	}
	return -1;
}


void UComponent_MasterResourceManager::_UpdateResourceDisplay(TSubclassOf<AResource_Base> _ResourceClass)
{
}

