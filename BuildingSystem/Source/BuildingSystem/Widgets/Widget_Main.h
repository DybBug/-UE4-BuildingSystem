// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_Main.generated.h"

class UComponent_MasterResourceManager;
class UComponent_PlayerResource;
class UComponent_BuildingManager;
class UWidget_StatBar;
class UWidget_Resource;
class UWidget_Building;
class UHorizontalBox;
class USizeBox;
class UImage;
class UTextBlock;

/**
 * 
 */
UCLASS()
class BUILDINGSYSTEM_API UWidget_Main : public UUserWidget
{
	GENERATED_BODY()
	
public :
	 const TArray<UWidget_Resource*>& GenerateResourceWidgets(UComponent_MasterResourceManager* _pManager);
	 const TArray<UWidget_Building*>& GenerateBuildingWidgets(UComponent_BuildingManager* _pManager);

	 void UpdateCurrentResource(UComponent_PlayerResource* _pPlayerResource);
	 void ShowCurrentResource(bool _bShow);

	/* Get */
	UWidget_StatBar* const& GetShildBar()                  const { return m_pShieldBar; }
	UWidget_StatBar* const& GetHealthBar()                 const { return m_pHealthBar;}
	UHorizontalBox*  const& GetResourceBox()               const { return m_pResourceBox; }
	USizeBox*        const& GetCurrentResourceBox()        const { return m_pCurrentResourceBox; }
	UImage*          const& GetCurrentResourceIcon()       const { return m_pCurrentResourceIcon; }
	UTextBlock*      const& GetCurrentResourceAmountText() const { return m_pCurrentResourceAmountText; }
	UImage*          const& GetStateIcon()                 const { return m_pStateIcon; }
	UImage*          const& GetCrosshair()                 const { return m_pCrosshair; }
	UHorizontalBox*  const& GetBuildingBox()               const { return m_pBuildingBox; }


protected :
	UPROPERTY()
	UWidget_StatBar* m_pShieldBar;
	UPROPERTY()
	UWidget_StatBar* m_pHealthBar;
	UPROPERTY()
	UHorizontalBox* m_pResourceBox;
	UPROPERTY()
	USizeBox* m_pCurrentResourceBox;
	UPROPERTY()
	UImage* m_pCurrentResourceIcon;
	UPROPERTY()
	UTextBlock* m_pCurrentResourceAmountText;
	UPROPERTY()
	UImage* m_pStateIcon;
	UPROPERTY()
	UImage* m_pCrosshair;
	UPROPERTY()
	UHorizontalBox* m_pBuildingBox;

	TArray<UWidget_Resource*> m_ResourceWidgets;

	TArray<UWidget_Building*> m_BuildingWidgets;
	
};
