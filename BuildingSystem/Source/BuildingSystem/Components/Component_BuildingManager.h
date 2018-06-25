// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../BuildingSystemStructure.h"
#include "Component_BuildingManager.generated.h"

class ABuilding_Base;
class APlayerCharacter;
class UWidget_Building;
class AResource_Base;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGSYSTEM_API UComponent_BuildingManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UComponent_BuildingManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public :
	void Initialize(APlayerCharacter* _pPlayer);

	void WidgetForBuilding(const TSubclassOf<ABuilding_Base>& _BuildingClass, bool& out_bSuccess, UWidget_Building*& out_pWidget);

	void OnStartBuilding();
	void StartBuilding();
	void ChangeCurrentBuilding();
	void OnStopBuilding();

	const FBuildingData& GetCurrentBuildingInfo();

	void SelectCurrentBuilding(bool _bSelect);
	void SelectBuildingByIndex(int _Index);

	FVector ToGridLocation(const FVector& _Location);
	int ToSnapRotation(float _In);
	FVector GetPlayerLookAtLocation(int _ForwardOffset);
	void GetBuildingPosAndRot(FVector& out_Location, FRotator& out_Rotation);

	void CnaBuildBuilding(bool* out_bCan = nullptr, TSubclassOf<AResource_Base>* out_ResourceClass = nullptr, int* out_Amount = nullptr);
	void OnResourceChanged();
	void OnResourceValueModified(TSubclassOf<AResource_Base> _ResourceClass);

	void PlaceCurrentBuilding();

	/* Get */
	const TArray<TSubclassOf<ABuilding_Base>>& GetBuildingClasses() const { return m_BuildingClasses; }

	const int&     GetForwardBuildingOffset() const { return m_ForwardBuildingOffset; }
	const FVector& GetLocationOffset()        const { return m_LocationOffset; }
		
protected :
	UPROPERTY(EditAnywhere, Category = "UComponent_BuildingManager")
	TArray<TSubclassOf<ABuilding_Base>> m_BuildingClasses;

	UPROPERTY(EditAnywhere, Category = "UComponent_BuildingManager|BuildingGrid")
	int m_XGridSize = 200;
	UPROPERTY(EditAnywhere, Category = "UComponent_BuildingManager|BuildingGrid")
	int m_YGridSize = 200;
	UPROPERTY(EditAnywhere, Category = "UComponent_BuildingManager|BuildingGrid")
	int m_ZGridSize = 400;
	UPROPERTY(EditAnywhere, Category = "UComponent_BuildingManager|BuildingGrid")
	int m_RotationSnap = 90;
	UPROPERTY(EditAnywhere, Category = "UComponent_BuildingManager|BuildingGrid")
	int m_ForwardBuildingOffset = 200;
	UPROPERTY(EditAnywhere, Category = "UComponent_BuildingManager|BuildingGrid")
	FVector m_LocationOffset = FVector(0.f, 0.f, 40.f);
	UPROPERTY(EditAnywhere, Category = "UComponent_BuildingManager|BuildingGrid")
	int m_RotationOffset = 90;

	APlayerCharacter* m_pPlayer;

	bool m_bInitialized;

	TArray<UWidget_Building*> m_BuildingWidgets;

	int m_SelectedBuildingIndex;

	bool m_bCurrentlyBuilding;

	float m_BuildingUpdateInterval = 0.03f;

	ABuilding_Base* m_pCurrentBuilding;

	FTimerHandle m_hBuildingTimer;

};

