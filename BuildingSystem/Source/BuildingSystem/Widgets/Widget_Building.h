// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_Building.generated.h"

class UImage;
class UBorder;
class UTextBlock;
class USpacer;
class ABuilding_Base;
/**
 * 
 */
UCLASS()
class BUILDINGSYSTEM_API UWidget_Building : public UUserWidget
{
	GENERATED_BODY()
	
protected :
	virtual void NativeConstruct() override;

public :
	void InitializeWidget(const TSubclassOf<ABuilding_Base>& _BuildingClass);
	void SelectBuilding(bool _bSelect);

	/* Get */
	UTextBlock* const& GetHotkeyText() const { return m_pHotkeyText; }
	UBorder*    const& GetOutline()    const { return m_pOutline; }
	UImage*     const& GetIcon()       const { return m_pIcon; }
	USpacer*    const& GetIconSpacer() const { return m_pIconSpacer; }

	const TSubclassOf<ABuilding_Base>& GetBuildingClass() const { return m_BuildingClass; }

protected :
	UPROPERTY()
	UTextBlock* m_pHotkeyText;
	UPROPERTY()
	UBorder* m_pOutline;
	UPROPERTY()
	UImage* m_pIcon;
	UPROPERTY()
	USpacer* m_pIconSpacer;

	bool m_bSelected;
	TSubclassOf<ABuilding_Base> m_BuildingClass;
};
