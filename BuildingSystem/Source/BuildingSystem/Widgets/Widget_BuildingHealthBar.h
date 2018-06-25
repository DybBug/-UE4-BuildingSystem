// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_BuildingHealthBar.generated.h"

enum class EBuildingStates : uint8;
class UProgressBar;
class UTextBlock;
/**
 * 
 */
UCLASS()
class BUILDINGSYSTEM_API UWidget_BuildingHealthBar : public UUserWidget
{
	GENERATED_BODY()
	
public :
	void InitializeWidget(int _MaxHealth);
	void UpdateHealth(int _NewValue);
	void SetState(EBuildingStates _NewState);

	/* Get */
	UProgressBar* const& GetHealthBar()  const { return m_pHealthBar; }
	UTextBlock*   const& GetHealthText() const { return m_pHealthText; }

protected :
	UPROPERTY()
	UProgressBar* m_pHealthBar;
	UPROPERTY()
	UTextBlock* m_pHealthText;

	EBuildingStates m_State;
	int m_CurrentHealth;
	int m_MaxHealth;
	
};
