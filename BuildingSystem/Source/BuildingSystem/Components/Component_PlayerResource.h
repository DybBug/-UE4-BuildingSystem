// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Component_MasterResourceManager.h"
#include "Component_PlayerResource.generated.h"

class APlayerCharacter;
class UWidget_Resource;
class AResource_Base;
/**
 * 
 */
UCLASS()
class BUILDINGSYSTEM_API UComponent_PlayerResource : public UComponent_MasterResourceManager
{
	GENERATED_BODY()
	
public :
	void InitializeManager(APlayerCharacter* _pPlayer);

	TSubclassOf<AResource_Base> GetSelectedResource();

	void GetWidgetForResource(
		const TSubclassOf<AResource_Base>& _ResourceClass, 
		bool& out_bSuccess, 
		UWidget_Resource*& out_pWidget);

	UWidget_Resource* CurrentlySelectedWidget();

	void SelectCurrentResource();

	void SelectNextResource();

	/* Get */
	const int& GetSelectedResourceIndex() const { return m_SelectedResourceIndex; }

protected :
	// 화면에 해당 재료의 수량 업데이트.
	virtual void _UpdateResourceDisplay(TSubclassOf<AResource_Base> _ResourceClass) override;

	void _OnResourceSwitched();
	
protected :
	TArray<UWidget_Resource*> m_ResourceWidgets;

	APlayerCharacter* m_pPlayer;
	bool m_bInitialized;

	int m_SelectedResourceIndex;
};
