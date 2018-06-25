// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget_Main.h"
#include "Widget_Resource.h"
#include "Widget_Building.h"
#include "../Components/Component_PlayerResource.h"
#include "../Components/Component_BuildingManager.h"
#include "../Resources/Resource_Base.h"


#include <Components/HorizontalBox.h>
#include <Components/SizeBox.h>
#include <Components/Image.h>
#include <Components/TextBlock.h>



const TArray<UWidget_Resource*>& UWidget_Main::GenerateResourceWidgets(UComponent_MasterResourceManager* _pManager)
{
	UClass* pWidgetClass = LoadClass<UWidget_Resource>(nullptr, TEXT("WidgetBlueprint'/Game/BuildingSystem/Blueprints/Widgets/WB_Resource.WB_Resource_C'"));

	TArray<TSubclassOf<AResource_Base>> Keys;

	_pManager->GetResources().GetKeys(Keys);
	for (auto& Iterator : Keys)
	{
		UWidget_Resource* pWidget = CreateWidget<UWidget_Resource>(GWorld, pWidgetClass);
		pWidget->InitializeWidget(Iterator);

		m_ResourceWidgets.Add(pWidget);

		m_pResourceBox->AddChildToHorizontalBox(pWidget);
	}

	return m_ResourceWidgets;
}

const TArray<UWidget_Building*>& UWidget_Main::GenerateBuildingWidgets(UComponent_BuildingManager* _pManager)
{
	UClass* pWidgetClass = LoadClass<UWidget_Building>(nullptr, TEXT("WidgetBlueprint'/Game/BuildingSystem/Blueprints/Widgets/WB_Building.WB_Building_C'"));

	for (auto& Iterator : _pManager->GetBuildingClasses())
	{
		UWidget_Building* pWidget = CreateWidget<UWidget_Building>(GWorld, pWidgetClass);
		pWidget->InitializeWidget(Iterator);

		m_BuildingWidgets.Add(pWidget);

		m_pBuildingBox->AddChildToHorizontalBox(pWidget);
	}

	return m_BuildingWidgets;
}

void UWidget_Main::UpdateCurrentResource(UComponent_PlayerResource * _pPlayerResource)
{
	TSubclassOf<AResource_Base> SelectedResourceClass = _pPlayerResource->GetSelectedResource();

	UTexture2D* pIcon = SelectedResourceClass.GetDefaultObject()->GetResourceData().pIcon;
	m_pCurrentResourceIcon->SetBrushFromTexture(pIcon);

	int Amount = _pPlayerResource->GetResourceAmount(SelectedResourceClass);
	m_pCurrentResourceAmountText->SetText(FText::AsNumber(Amount));	
}

void UWidget_Main::ShowCurrentResource(bool _bShow)
{
	ESlateVisibility Visibility = _bShow ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden;
	m_pCurrentResourceBox->SetVisibility(Visibility);
}
