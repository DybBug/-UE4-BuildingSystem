// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../BuildingSystemStructure.h"
#include "Widget_Build.generated.h"

class UProgressBar;
class UTextBlock;
class UImage;
class AResource_Base;
/**
 * 
 */
UCLASS()
class BUILDINGSYSTEM_API UWidget_Build : public UUserWidget
{
	GENERATED_BODY()
	
public :
	void Update(const FBuildingResourceVersion& _VersionInfo,  const TSubclassOf<AResource_Base>& _ResourceClass);
	void SetCanBeBuilt(bool _NewValue);

	/* Get */
	UProgressBar* const& GetProgerssBar()        const { return m_pProgressBar; }
	UTextBlock*   const& GetHelathText()         const { return m_pHealthText; }
	UImage*       const& GetResourceIcon()       const { return m_pResourceIcon; }
	UTextBlock*   const& GetResourceAmountText() const { return m_pResourceAmountText; }
protected :
	UPROPERTY()
	UProgressBar* m_pProgressBar;
	UPROPERTY()
	UTextBlock* m_pHealthText;
	UPROPERTY()
	UImage* m_pResourceIcon;
	UPROPERTY()
	UTextBlock* m_pResourceAmountText;
	
	bool m_bCanBeBuilt = true;

};
