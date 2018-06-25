// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget_Build.h"
#include "../Resources/Resource_Base.h"

 #include <Components/ProgressBar.h>
 #include <Components/TextBlock.h>
 #include <Components/Image.h>


void UWidget_Build::Update(const FBuildingResourceVersion& _VersionInfo, const TSubclassOf<AResource_Base>& _ResourceClass)
{
#define LOCTEXT_NAMESPACE "HealthText"
	FText Format = FText::Format(LOCTEXT("HealthText", "{0} HP"), _VersionInfo.MaximumHealth);
#undef LOCTEXT_NAMESPACE
	m_pHealthText->SetText(Format);

	m_pResourceAmountText->SetText(FText::AsNumber(_VersionInfo.RequireAmount));

	m_pResourceIcon->SetBrushFromTexture(_ResourceClass.GetDefaultObject()->GetResourceData().pIcon);
}

void UWidget_Build::SetCanBeBuilt(bool _NewValue)
{
	if (m_bCanBeBuilt != _NewValue)
	{
		m_bCanBeBuilt = _NewValue;

		FLinearColor Color = m_bCanBeBuilt ? FColor(0xFFFFFFFF) : FColor(0xFFFF393F);

		m_pResourceIcon->SetColorAndOpacity(Color);

		m_pResourceAmountText->SetColorAndOpacity(Color);
	}
}
