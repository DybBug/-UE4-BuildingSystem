// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../BuildingSystemStructure.h"
#include "Widget_StatBar.generated.h"

class UImage;
class UProgressBar;
class UTextBlock;
/**
 * 
 */
UCLASS()
class BUILDINGSYSTEM_API UWidget_StatBar : public UUserWidget
{
	GENERATED_BODY()

protected :
	virtual void NativePreConstruct() override;

public :
	void InitializeBar(const FStatValue& _StatValue);

	// #. 현재 체력, 프로그래스바의 퍼센트 업데이트.
	void UpdateStat(int _NewValue);

	/* Get */
	UImage*       const& GetIcon()        const { return m_pIcon; }
	UProgressBar* const& GetBar()         const { return m_pBar; }
	UTextBlock*   const& GetCurrentText() const { return m_pCurrentText; }
	UTextBlock*   const& GetMaxText()     const { return m_pMaxText; }
	
private :
	UPROPERTY()
	UImage* m_pIcon;
	UPROPERTY()
	UProgressBar* m_pBar;
	UPROPERTY()
	UTextBlock* m_pCurrentText;
	UPROPERTY()
	UTextBlock* m_pMaxText;

	UPROPERTY(EditAnywhere, Category = "UWidget_StatBar|Appearance")
	UTexture2D* m_pStatIcon;
	UPROPERTY(EditAnywhere, Category = "UWidget_StatBar|Appearance")
	FLinearColor m_BarColor;

	int m_MinValue;
	int m_CurrentValue;
	int m_MaxValue;
	bool m_bInitialized;

	
};
