// Fill out your copyright notice in the Description page of Project Settings.

#include "Building_Base.h"
#include "../Resources/Resource_Base.h"
#include "../Widgets/Widget_Build.h"
#include "../Widgets/Widget_BuildingHealthBar.h"
#include "../Components/Component_BuildingStats.h"

#include <Components/SceneComponent.h>
#include <Components/BoxComponent.h>
#include <Components/StaticMeshComponent.h>
#include <UObject/ConstructorHelpers.h>
#include <WidgetComponent.h>
#include <Materials/MaterialInstanceDynamic.h>
#include <Engine/World.h>
#include <TimerManager.h>
#include <Kismet/KismetSystemLibrary.h>



// Sets default values
ABuilding_Base::ABuilding_Base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	m_pScene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = m_pScene;

	m_pBuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildingMesh"));
	m_pBuildingMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	m_pBuildingMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	m_pBuildingMesh->SetupAttachment(RootComponent);

	m_pBoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollison"));
	m_pBoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	m_pBoxCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	m_pBoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ABuilding_Base::_OnBeginOverlap);
	m_pBoxCollision->OnComponentEndOverlap.AddDynamic(this, &ABuilding_Base::_OnEndOverlap);
	m_pBoxCollision->SetupAttachment(RootComponent);

	static ConstructorHelpers::FClassFinder<UWidget_Build> WB_Build(TEXT("WidgetBlueprint'/Game/BuildingSystem/Blueprints/Widgets/WB_Build.WB_Build_C'"));
	if (WB_Build.Succeeded())
	{
		m_pBuildWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("BuildWidget"));
		m_pBuildWidget->SetWidgetSpace(EWidgetSpace::Screen);
		m_pBuildWidget->SetWidgetClass(WB_Build.Class);
		m_pBuildWidget->SetDrawSize(FVector2D(380.f, 140.f));
		m_pBuildWidget->SetupAttachment(RootComponent);
	}

	static ConstructorHelpers::FClassFinder<UWidget_BuildingHealthBar> WB_BuildingHealthBar(TEXT("WidgetBlueprint'/Game/BuildingSystem/Blueprints/Widgets/WB_BuildingHealthBar.WB_BuildingHealthBar_C'"));
	if (WB_BuildingHealthBar.Succeeded())
	{
		m_pHealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
		m_pHealthBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
		m_pHealthBarWidget->SetWidgetClass(WB_BuildingHealthBar.Class);
		m_pHealthBarWidget->SetDrawSize(FVector2D(360.f, 32.f));
		m_pHealthBarWidget->SetVisibility(false);
		m_pHealthBarWidget->SetupAttachment(RootComponent);
	}
	
	m_pBuildingStats = CreateDefaultSubobject<UComponent_BuildingStats>(TEXT("BuildingStats"));


	static ConstructorHelpers::FClassFinder<AResource_Base> BP_Resource_Wood(TEXT("Blueprint'/Game/BuildingSystem/Blueprints/Resources/BP_Resource_Wood.BP_Resource_Wood_C'"));
	static ConstructorHelpers::FClassFinder<AResource_Base> BP_Resource_Stone(TEXT("Blueprint'/Game/BuildingSystem/Blueprints/Resources/BP_Resource_Stone.BP_Resource_Stone_C'"));
	static ConstructorHelpers::FClassFinder<AResource_Base> BP_Resource_Metal(TEXT("Blueprint'/Game/BuildingSystem/Blueprints/Resources/BP_Resource_Metal.BP_Resource_Metal_C'"));

	m_ResourceVersions.Emplace(BP_Resource_Wood.Class, FBuildingResourceVersion());
	m_ResourceVersions.Emplace(BP_Resource_Stone.Class, FBuildingResourceVersion());
	m_ResourceVersions.Emplace(BP_Resource_Metal.Class, FBuildingResourceVersion());


}

// Called when the game starts or when spawned
void ABuilding_Base::BeginPlay()
{
	Super::BeginPlay();
}

void ABuilding_Base::Initialize(const TSubclassOf<AResource_Base>& _ResourceClass)
{
	m_ResourceClass = _ResourceClass;

	m_pDynamicMaterial = m_pBuildingMesh->CreateDynamicMaterialInstance(0);	

	UppdateGhostMaterial();
	
	m_pBuildWidget_Ref     = Cast<UWidget_Build>(m_pBuildWidget->GetUserWidgetObject());
	m_pHealthBarWidget_Ref = Cast<UWidget_BuildingHealthBar>(m_pHealthBarWidget->GetUserWidgetObject());

	FBuildingResourceVersion* pVersion = m_ResourceVersions.Find(m_ResourceClass);
	m_pBuildWidget_Ref->Update(*pVersion, m_ResourceClass);

	m_pBuildingStats->InitializeManager(this);

	m_pBuildWidget->SetVisibility(true);
}

void ABuilding_Base::UppdateGhostMaterial()
{
	FLinearColor Color = m_ResourceClass.GetDefaultObject()->GetResourceData().GhostBuildingColor;
	m_pDynamicMaterial->SetVectorParameterValue(TEXT("Color"), Color);
}

void ABuilding_Base::ChangeResource(TSubclassOf<AResource_Base> _NewResourceClass)
{
	if (_NewResourceClass != nullptr)
	{
		if (m_ResourceClass != _NewResourceClass)
		{
			m_ResourceClass = _NewResourceClass;

			UppdateGhostMaterial();

			FBuildingResourceVersion* pVersion = m_ResourceVersions.Find(m_ResourceClass);
			m_pBuildWidget_Ref->Update(*pVersion, m_ResourceClass);
		}
	}
}

void ABuilding_Base::SetCanBeBuilt(bool _bNewValue)
{
	m_bCanBeBuilt = _bNewValue;
	m_pBuildWidget_Ref->SetCanBeBuilt(m_bCanBeBuilt);
}

void ABuilding_Base::OnBuild(const TSubclassOf<AResource_Base>& _ResourceClass)
{
	if(_ResourceClass)
	{
		m_bBuilt = true;

		m_ResourceClass = _ResourceClass;
				
		m_pBuildWidget->DestroyComponent();

		int MaximumHealth = m_ResourceVersions.Find(_ResourceClass)->MaximumHealth;
		m_pBuildingStats->SetMaxStatValue(EStats::Health, MaximumHealth, false);
		m_pBuildingStats->SetStat(EStats::Health, 0, false);
		
		m_BuildingTime = m_ResourceVersions.Find(_ResourceClass)->BuildingTime;

		m_pHealthBarWidget_Ref->InitializeWidget(m_pBuildingStats->GetMaxHealth());

		m_pHealthBarWidget->SetVisibility(true);

		GetWorld()->GetTimerManager().SetTimer(m_hBuildingTimer, this, &ABuilding_Base::BuildingTick, m_Interval, true);
		
		FLatentActionInfo Latent;
		Latent.CallbackTarget = this;
		Latent.ExecutionFunction = TEXT("OnBuildingEnd");
		Latent.UUID = GetUniqueID();
		UKismetSystemLibrary::Delay(GetWorld(), m_BuildingTime + m_Interval, Latent);
	}
}

void ABuilding_Base::BuildingTick()
{
	int MaxHealth = m_pBuildingStats->GetMaxHealth();
	int Value = FMath::FloorToInt(m_BuildingTime / m_Interval);
	m_pBuildingStats->ModifyStat(EStats::Health, FMath::CeilToInt((float)MaxHealth / (float)Value));

	int CurrentValue;
	m_pBuildingStats->GetStat(EStats::Health, &CurrentValue, nullptr);
	if (CurrentValue == MaxHealth)
	{
		OnBuildingEnd();
	}
}

void ABuilding_Base::OnBuildingEnd()
{
	if (m_hBuildingTimer.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(m_hBuildingTimer);

		m_pBuildingMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		m_pBuildingMesh->SetMaterial(0, m_ResourceClass.GetDefaultObject()->GetResourceData().pBuildingMaterial);
		
		 m_pHealthBarWidget_Ref->SetState(EBuildingStates::Built);
	}
}

void ABuilding_Base::OnEnterPlayerSight()
{
	m_bInPlayerSight = true;
	if (m_bBuilt)
	{
		int CurrHealth;
		m_pBuildingStats->GetStat(EStats::Health, &CurrHealth, nullptr);
		m_pHealthBarWidget_Ref->UpdateHealth(CurrHealth);
		m_pHealthBarWidget->SetVisibility(true);
	}
}

void ABuilding_Base::OnLeavePlayerSight()
{
	m_bInPlayerSight= false;
	if (m_bBuilt)
	{
		m_pHealthBarWidget->SetVisibility(false);
	}
}

void ABuilding_Base::_OnBeginOverlap(UPrimitiveComponent* _pOverlappedComponent, AActor* _pOtherActor, UPrimitiveComponent* _pOtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult)
{
	if (_pOtherActor != this)
	{
		UStaticMeshComponent* pOverlappedStaticMesh = Cast<UStaticMeshComponent>(_pOverlappedComponent);
		ABuilding_Base* pOverlappedBuilding = Cast<ABuilding_Base>(_pOtherActor);

		if (pOverlappedBuilding && pOverlappedBuilding->GetBuilt())
		{
			m_OverlappingBuildings.Emplace(pOverlappedBuilding);

			if (!m_bOverlappinng)
			{
				_OnOverlappingBegin();
			}
		}		
	}
}

void ABuilding_Base::_OnEndOverlap(UPrimitiveComponent* _pOverlappedComponent, AActor* _pOtherActor, UPrimitiveComponent* _pOtherComp, int32 _OtherBodyIndex)
{
	if (_pOtherActor != this)
	{
		UStaticMeshComponent* pOverlappedStaticMesh = Cast<UStaticMeshComponent>(_pOverlappedComponent);
		ABuilding_Base* pOverlappedBuilding = Cast<ABuilding_Base>(_pOtherActor);

		if (pOverlappedBuilding)
		{
			if (m_OverlappingBuildings.Contains(pOverlappedBuilding))
			{
				m_OverlappingBuildings.Remove(pOverlappedBuilding);

				if (m_OverlappingBuildings.Array().Num() <= 0)
				{
					_OnOverlappingEnd();
				}
			}
		}
	}
}

void ABuilding_Base::_OnOverlappingBegin()
{
	m_bOverlappinng = true;
	m_pBuildWidget->SetVisibility(false);
	m_pBuildingMesh->SetVisibility(false);
}

void ABuilding_Base::_OnOverlappingEnd()
{
	m_bOverlappinng = false;
	m_pBuildWidget->SetVisibility(true);
	m_pBuildingMesh->SetVisibility(true);
}

