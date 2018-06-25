// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../BuildingSystemStructure.h"
#include "Building_Base.generated.h"

class AResource_Base;
class UBoxComponent;
class UWidgetComponent;
class UWidget_Build;
class UComponent_BuildingStats;
class UWidget_BuildingHealthBar;

UCLASS()
class BUILDINGSYSTEM_API ABuilding_Base : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABuilding_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void Initialize(const TSubclassOf<AResource_Base>& _ResourceClass);
	void UppdateGhostMaterial();
	void ChangeResource(TSubclassOf<AResource_Base> _NewResourceClass);

	void SetCanBeBuilt(bool _bNewValue);

	void OnBuild(const TSubclassOf<AResource_Base>& _ResourceClass);

	void BuildingTick();
	void OnBuildingEnd();

	void OnEnterPlayerSight();
	void OnLeavePlayerSight();

	/* Get */
	const FBuildingData& GetBuildingData()  const { return m_BuildingData; }
	const bool&          GetInPlayerSight() const { return m_bInPlayerSight; }
	const bool&          GetOverlapping()   const { return m_bOverlappinng; }
	const bool&          GetBuilt()         const { return m_bBuilt; }

	UWidget_Build*             const& GetBuildWidget_Ref()     const { return m_pBuildWidget_Ref; }
	UWidget_BuildingHealthBar* const& GetHealthBarWidget_Ref() const { return m_pHealthBarWidget_Ref; }

	const TMap<TSubclassOf<AResource_Base>, FBuildingResourceVersion>& GetResourceVersions() const { return m_ResourceVersions; }

protected :
	UFUNCTION()
	void _OnBeginOverlap( 
		UPrimitiveComponent* _pOverlappedComponent,
		AActor* _pOtherActor,
		UPrimitiveComponent* _pOtherComp, 
		int32 _OtherBodyIndex, 
		bool _bFromSweep, 
		const FHitResult& _SweepResult);

	UFUNCTION()
	void _OnEndOverlap(
		UPrimitiveComponent* _pOverlappedComponent, 
		AActor* _pOtherActor, 
		UPrimitiveComponent* _pOtherComp, 
		int32 _OtherBodyIndex);

	void _OnOverlappingBegin();
	void _OnOverlappingEnd();

protected:
	UPROPERTY(VisibleAnywhere, Category = "ABuilding_Base|Components")
	USceneComponent* m_pScene;

	UPROPERTY(VisibleAnywhere, Category = "ABuilding_Base|Components")
	UStaticMeshComponent* m_pBuildingMesh;

	UPROPERTY(VisibleAnywhere, Category = "ABuilding_Base|Components")
	UBoxComponent* m_pBoxCollision;

	UPROPERTY(VisibleAnywhere, Category = "ABuilding_Base|Components")
	UWidgetComponent* m_pBuildWidget;

	UPROPERTY(VisibleAnywhere, Category = "ABuilding_Base|Components")
	UWidgetComponent* m_pHealthBarWidget;

	UPROPERTY(VisibleAnywhere, Category = "ABuilding_Base|Components")
	UComponent_BuildingStats* m_pBuildingStats;


	UPROPERTY(EditAnywhere, Category = "ABuilding_Base|Data")
	FBuildingData m_BuildingData;

	UPROPERTY(EditAnywhere, Category = "ABuilding_Base|Data")
	TMap<TSubclassOf<AResource_Base>, FBuildingResourceVersion> m_ResourceVersions;

	TSubclassOf<AResource_Base> m_ResourceClass;

	UMaterialInstanceDynamic* m_pDynamicMaterial;

	UWidget_Build*             m_pBuildWidget_Ref;
	UWidget_BuildingHealthBar* m_pHealthBarWidget_Ref;

	bool m_bCanBeBuilt;

	bool m_bBuilt;
	bool m_bInPlayerSight = true;

	FTimerHandle m_hBuildingTimer;
	float m_BuildingTime;
	float m_Interval = 0.25f;

	TSet<ABuilding_Base*> m_OverlappingBuildings;
	bool m_bOverlappinng;

};
