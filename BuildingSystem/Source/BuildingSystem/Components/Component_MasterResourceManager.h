// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Component_MasterResourceManager.generated.h"


class AResource_Base;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGSYSTEM_API UComponent_MasterResourceManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UComponent_MasterResourceManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public :
	void AddResource(TSubclassOf<AResource_Base> _ResourceClass, int _AddAmount);
	void RemoveResource(TSubclassOf<AResource_Base> _ResourceClass, int _RemoveAmount);

	// #. 해당 자원의 수량을 리턴한다.
	int GetResourceAmount(TSubclassOf<AResource_Base> _ResourceClass);

	/* Get */
	const TMap<TSubclassOf<AResource_Base>, int>& GetResources() const { return m_Resources; }

protected :
	// 화면에 해당 재료의 수량 업데이트.
	virtual void _UpdateResourceDisplay(TSubclassOf<AResource_Base> _ResourceClass);

protected :
	// #. 자원의 종류와 수량.
	UPROPERTY(EditAnywhere, Category = "Default")
	TMap<TSubclassOf<AResource_Base>, int> m_Resources;
	
	int m_MaxAmount = 999;
};
