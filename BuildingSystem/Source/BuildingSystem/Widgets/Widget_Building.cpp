// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget_Building.h"
#include "../Buildings/Building_Base.h"

#include <Components/Image.h>
#include <Components/Border.h>
#include <Components/TextBlock.h>
#include <Components/Spacer.h>

void UWidget_Building::NativeConstruct()
{
	Super::NativeConstruct();

	FBuildingData Data = m_BuildingClass.GetDefaultObject()->GetBuildingData();

	FText KeyName = Data.HotKey.GetDisplayName();
	m_pHotkeyText->SetText(KeyName);
	
	UTexture2D* pIcon = Data.pIcon;
	m_pIcon->SetBrushFromTexture(pIcon, true);
	m_pIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	SelectBuilding(false);

	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UWidget_Building::InitializeWidget(const TSubclassOf<ABuilding_Base>& _BuildingClass)
{
	m_BuildingClass = _BuildingClass;
}

void UWidget_Building::SelectBuilding(bool _bSelect)
{
	if (m_bSelected != _bSelect)
	{
		m_bSelected = _bSelect;

		FLinearColor Color = m_bSelected ? FColor(0xFFFF0000) : FColor(0xFFFFFFFF);
		m_pOutline->SetBrushColor(Color);

		float SizeY = m_bSelected ? 5.f : 10.f;
		m_pIconSpacer->SetSize(FVector2D(1.f, SizeY));
	}
}


