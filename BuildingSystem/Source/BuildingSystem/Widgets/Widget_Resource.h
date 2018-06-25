// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_Resource.generated.h"

class UBorder;
class UImage;
class UTextBlock;
class AResource_Base;
/**
 * 
 */
UCLASS()
class BUILDINGSYSTEM_API UWidget_Resource : public UUserWidget
{
	GENERATED_BODY()

protected :
	virtual void NativeConstruct() override;

public :
	void InitializeWidget(const TSubclassOf<AResource_Base>& _ResourceClass);
	void SelectResource(bool _bSelected, bool _bIgnoreCheck);

	/* Get */
	UBorder*    const& GetResourceBorder() const { return m_pResourceBorder; }
	UImage*     const& GetIcon()           const { return m_pIcon; }
	UTextBlock* const& GetAmountText()     const { return m_pAmountText; }

	const TSubclassOf<AResource_Base>& GetResourceClass() const { return m_ResourceClass; }
	
protected :
	UPROPERTY()
	UBorder* m_pResourceBorder;
	UPROPERTY()
	UImage* m_pIcon;
	UPROPERTY()
	UTextBlock* m_pAmountText;

	bool m_bSelected;
	TSubclassOf<AResource_Base> m_ResourceClass;	
};
