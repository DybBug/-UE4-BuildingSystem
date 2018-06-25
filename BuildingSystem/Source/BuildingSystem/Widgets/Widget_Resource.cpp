// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget_Resource.h"
#include "../Resources/Resource_Base.h"

#include <Components/Border.h>
#include <Components/Image.h>
#include <Components/TextBlock.h>
#include <Styling/SlateColor.h>



void UWidget_Resource::NativeConstruct()
{
	UUserWidget::NativeConstruct();

	UTexture2D* pIcon = m_ResourceClass.GetDefaultObject()->GetResourceData().pIcon;
	m_pIcon->SetBrushFromTexture(pIcon);
	m_pIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UWidget_Resource::InitializeWidget(const TSubclassOf<AResource_Base>& _ResourceClass)
{
	m_ResourceClass = _ResourceClass;
}

void UWidget_Resource::SelectResource(bool _bSelected, bool _bIgnoreCheck)
{
	if ((m_bSelected != _bSelected) || _bIgnoreCheck)
	{
		m_bSelected = _bSelected;

		FColor Color = m_bSelected ? FColor(0x99060B1E) : FColor(0x3F000000);
		m_pResourceBorder->SetBrushColor(FLinearColor(Color));

		Color = m_bSelected ? FColor(0xFFFFFFFF) : FColor(0xCCFFFFFF);		
		m_pAmountText->SetColorAndOpacity(FLinearColor(Color));
	}
}
