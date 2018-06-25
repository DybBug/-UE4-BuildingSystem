// Fill out your copyright notice in the Description page of Project Settings.

#include "Component_PlayerResource.h"
#include "Component_BuildingManager.h"
#include "../Widgets/Widget_Resource.h"
#include "../Widgets/Widget_Main.h"
#include "../Characters/PlayerCharacter.h"
#include "../Resources/Resource_Base.h"
#include "../BuildingSystemEnums.h"

#include <Components/TextBlock.h>


void UComponent_PlayerResource::InitializeManager(APlayerCharacter* _pPlayer)
{
	if (_pPlayer)
	{
		m_pPlayer = _pPlayer;
		m_bInitialized = true;

		m_ResourceWidgets = m_pPlayer->GetMainWidget()->GenerateResourceWidgets(this);
		
		TArray<TSubclassOf<AResource_Base>> Keys;
		m_Resources.GetKeys(Keys);
		for (auto& Iterator : Keys)
		{
			_UpdateResourceDisplay(Iterator);
		}
		SelectCurrentResource();
	}
}


TSubclassOf<AResource_Base> UComponent_PlayerResource::GetSelectedResource()
{
	TArray<TSubclassOf<AResource_Base>> Keys;
	m_Resources.GetKeys(Keys);

	return Keys[m_SelectedResourceIndex];
}

void UComponent_PlayerResource::GetWidgetForResource(
	const TSubclassOf<AResource_Base>& _ResourceClass, 
	bool& out_bSuccess, 
	UWidget_Resource*& out_pWidget)
{
	UWidget_Resource* FoundWidget = nullptr;

	for (auto& Iterator : m_ResourceWidgets)
	{
		if (Iterator->GetResourceClass() == _ResourceClass)
		{
			FoundWidget = Iterator;
			break;
		}
	}

	// #. 성공했을때 값 리턴.
	if (FoundWidget)
	{
		out_bSuccess = true;
		out_pWidget = FoundWidget;
		return;
	}
	
	//#. 실패했을 때 값 리턴.
	out_bSuccess = false;
	out_pWidget = nullptr;
}

UWidget_Resource*  UComponent_PlayerResource::CurrentlySelectedWidget()
{
	// #. 선택된 리소스의 클래스.
	TSubclassOf<AResource_Base> ResourceClass = GetSelectedResource();

	bool bSuccess;
	UWidget_Resource* pResourceWidget;
	// #. 해당 리소스 클래스를 가지고 있는 위젯.
	GetWidgetForResource(ResourceClass, bSuccess, pResourceWidget);

	return pResourceWidget;
}

void UComponent_PlayerResource::SelectCurrentResource()
{
	UWidget_Resource* pResourceWidget = CurrentlySelectedWidget();
	pResourceWidget->SelectResource(true, false);
}

void UComponent_PlayerResource::SelectNextResource()
{
	CurrentlySelectedWidget()->SelectResource(false, false);

	TArray<TSubclassOf<AResource_Base>> Keys;
	m_Resources.GetKeys(Keys);

	m_SelectedResourceIndex = (m_SelectedResourceIndex+1) % Keys.Num();
	
	SelectCurrentResource();

	m_pPlayer->GetMainWidget()->UpdateCurrentResource(this);

	_OnResourceSwitched();
}

void UComponent_PlayerResource::_UpdateResourceDisplay(TSubclassOf<AResource_Base> _ResourceClass)
{
	if (m_bInitialized)
	{
		bool bSuccess;
		UWidget_Resource* pResourceWidget;
		GetWidgetForResource(_ResourceClass, bSuccess, pResourceWidget);

		if (bSuccess)
		{
			int Amount = GetResourceAmount(_ResourceClass);
			pResourceWidget->GetAmountText()->SetText(FText::AsNumber(Amount));

			TSubclassOf<AResource_Base> ResourceClass = GetSelectedResource();
			if (ResourceClass == _ResourceClass &&
				m_pPlayer->GetState() == EPlayerStates::Construction)
			{
				m_pPlayer->GetMainWidget()->UpdateCurrentResource(this);
				m_pPlayer->GetBuildingManager()->OnResourceValueModified(_ResourceClass);
			}
		}
	}
}

void UComponent_PlayerResource::_OnResourceSwitched()
{
	m_pPlayer->GetBuildingManager()->OnResourceChanged();
}
