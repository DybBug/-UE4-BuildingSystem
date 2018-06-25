// Fill out your copyright notice in the Description page of Project Settings.

#include "Component_BuildingManager.h"
#include "Component_PlayerResource.h"
#include "../Characters/PlayerCharacter.h"
#include "../Widgets/Widget_Main.h"
#include "../Widgets/Widget_Building.h"
#include "../Buildings/Building_Base.h"
#include "../BuildingSystemEnums.h"
#include "../Resources/Resource_Base.h"

#include <Kismet/KismetMathLibrary.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Camera/CameraComponent.h>
#include <Engine/World.h>
#include <TimerManager.h>

// Sets default values for this component's properties
UComponent_BuildingManager::UComponent_BuildingManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UComponent_BuildingManager::BeginPlay()
{
	Super::BeginPlay();

	// ...	
}


void UComponent_BuildingManager::Initialize(APlayerCharacter* _pPlayer)
{
	if (_pPlayer)
	{
		m_pPlayer = _pPlayer;
		m_bInitialized = true;

		m_BuildingWidgets = m_pPlayer->GetMainWidget()->GenerateBuildingWidgets(this);

		if (m_pPlayer->GetState() == EPlayerStates::Construction)
		{
			SelectCurrentBuilding(true);
		}
	}
}

void UComponent_BuildingManager::WidgetForBuilding(const TSubclassOf<ABuilding_Base>& _BuildingClass, bool & out_bSuccess, UWidget_Building*& out_pWidget)
{
	for (auto& Iterator : m_BuildingWidgets)
	{
		if (Iterator->GetBuildingClass() == _BuildingClass)
		{
			out_bSuccess = true;
			out_pWidget = Iterator;
			return;
		}
	}

	out_bSuccess = false;
	out_pWidget = nullptr;
}

void UComponent_BuildingManager::OnStartBuilding()
{
	if (m_bCurrentlyBuilding)
	{
		ChangeCurrentBuilding();
	}
	else
	{
		StartBuilding();	
	}

	m_bCurrentlyBuilding = true;
}

void UComponent_BuildingManager::StartBuilding()
{
	FVector Location;
	FRotator Rotation;
	GetBuildingPosAndRot(Location, Rotation);
	m_pCurrentBuilding = GetWorld()->SpawnActor<ABuilding_Base>(m_BuildingClasses[m_SelectedBuildingIndex], Location, Rotation);
	m_pCurrentBuilding->Initialize(m_pPlayer->GetPlayerResources()->GetSelectedResource());

	bool bCan;

	CnaBuildBuilding(&bCan);
	m_pCurrentBuilding->SetCanBeBuilt(bCan);

	GetWorld()->GetTimerManager().SetTimer(m_hBuildingTimer, [&]()
	{
		FVector Loc;
		FRotator Rot;
		GetBuildingPosAndRot(Loc, Rot);
		m_pCurrentBuilding->SetActorLocationAndRotation(Loc, Rot);
	}, m_BuildingUpdateInterval, true);
}

void UComponent_BuildingManager::ChangeCurrentBuilding()
{
	GetWorld()->GetTimerManager().ClearTimer(m_hBuildingTimer);
	m_pCurrentBuilding->Destroy();
	m_pCurrentBuilding = nullptr;
	StartBuilding();
}

void UComponent_BuildingManager::OnStopBuilding()
{
	if (m_bCurrentlyBuilding)
	{
		m_bCurrentlyBuilding = false;
		SelectCurrentBuilding(false);

		if (m_pCurrentBuilding)
		{
			m_pCurrentBuilding->Destroy();
			m_pCurrentBuilding = nullptr;
			GetWorld()->GetTimerManager().ClearTimer(m_hBuildingTimer);
		}
	}
}

const FBuildingData& UComponent_BuildingManager::GetCurrentBuildingInfo()
{
	return m_BuildingClasses[m_SelectedBuildingIndex].GetDefaultObject()->GetBuildingData();
}

void UComponent_BuildingManager::SelectCurrentBuilding(bool _bSelect)
{
	bool bSuccess;
	UWidget_Building* pSelectedWidget = nullptr;
	WidgetForBuilding(m_BuildingClasses[m_SelectedBuildingIndex], bSuccess, pSelectedWidget);

	if (bSuccess)
	{
		pSelectedWidget->SelectBuilding(_bSelect);

		if (_bSelect)
		{
			OnStartBuilding();
		}
	}	
}

void UComponent_BuildingManager::SelectBuildingByIndex(int _Index)
{
	bool _bInRange = UKismetMathLibrary::InRange_IntInt(_Index, 0, m_BuildingClasses.Num()-1);
	if (_bInRange)
	{
		if (m_pPlayer->GetState() == EPlayerStates::Combat)
		{
			m_SelectedBuildingIndex = _Index;
			m_pPlayer->ChangeState(EPlayerStates::Construction);
		}
		else
		{
			if (m_SelectedBuildingIndex != _Index)
			{
				SelectCurrentBuilding(false);
				m_SelectedBuildingIndex = _Index;
				SelectCurrentBuilding(true);
			}
		}
	}
}

FVector UComponent_BuildingManager::ToGridLocation(const FVector& _Location)
{
	FVector Location;
	Location.X = FMath::RoundToInt(_Location.X / (float)m_XGridSize) * m_XGridSize;
	Location.Y = FMath::RoundToInt(_Location.Y / (float)m_YGridSize) * m_YGridSize;
	Location.Z = FMath::RoundToInt(_Location.Z / (float)m_ZGridSize) * m_ZGridSize;

	return Location;
}

int UComponent_BuildingManager::ToSnapRotation(float _In)
{
	return FMath::RoundToInt(_In / (float)m_RotationSnap) * m_RotationSnap;
}

FVector UComponent_BuildingManager::GetPlayerLookAtLocation(int _ForwardOffset)
{
	FVector PlayerLocation = m_pPlayer->GetActorLocation();
	FVector CameraForward = m_pPlayer->GetCamera()->GetForwardVector();

	//FVector LookAtLocation = (_ForwardOffset * CameraForward) + PlayerLocation;
	FVector LookAtLocation = (_ForwardOffset * CameraForward) + m_pPlayer->GetCamera()->GetComponentLocation();

	return LookAtLocation;
}

void UComponent_BuildingManager::GetBuildingPosAndRot(FVector& out_Location, FRotator& out_Rotation)
{
	out_Location = FVector::ZeroVector;
	out_Rotation = FRotator::ZeroRotator;

	FVector LookAtLocation = GetPlayerLookAtLocation(m_ForwardBuildingOffset);
	out_Location = m_LocationOffset + ToGridLocation(LookAtLocation);

	FRotator CameraRotInWorld = m_pPlayer->GetCamera()->GetComponentRotation();
	out_Rotation.Yaw = m_RotationOffset + ToSnapRotation(CameraRotInWorld.Yaw);

}

void UComponent_BuildingManager::CnaBuildBuilding(bool* out_bCan, TSubclassOf<AResource_Base>* out_ResourceClass, int* out_Amount)
{
	if (m_pCurrentBuilding)
	{
		TSubclassOf<AResource_Base> PlayerSelectedResourceClass = m_pPlayer->GetPlayerResources()->GetSelectedResource();

		int RequireAmount = m_pCurrentBuilding->GetResourceVersions().Find(PlayerSelectedResourceClass)->RequireAmount;

		int PlayerAmount = m_pPlayer->GetPlayerResources()->GetResourceAmount(PlayerSelectedResourceClass);

		if(out_bCan)          *out_bCan          = (PlayerAmount >= RequireAmount);
		if(out_ResourceClass) *out_ResourceClass = PlayerSelectedResourceClass;
		if(out_Amount)        *out_Amount        = RequireAmount;

		return;
	}

	if(out_bCan)          *out_bCan          = false;
	if(out_ResourceClass) *out_ResourceClass = nullptr;
	if(out_Amount)        *out_Amount        = 0;
}

void UComponent_BuildingManager::OnResourceChanged()
{
	if (m_bCurrentlyBuilding && m_bInitialized)
	{
		if (m_pCurrentBuilding)
		{
			TSubclassOf<AResource_Base> SelectedResourceClass = m_pPlayer->GetPlayerResources()->GetSelectedResource();
			m_pCurrentBuilding->ChangeResource(SelectedResourceClass);
			
			bool bCan;
			CnaBuildBuilding(&bCan);
			m_pCurrentBuilding->SetCanBeBuilt(bCan);
		}
	}
}

void UComponent_BuildingManager::OnResourceValueModified(TSubclassOf<AResource_Base> _ResourceClass)
{
	if (m_bCurrentlyBuilding &&
		m_bInitialized &&
		_ResourceClass &&
		m_pPlayer->GetPlayerResources()->GetSelectedResource() == _ResourceClass)
	{
		if (m_pCurrentBuilding)
		{
			bool bCan;
			CnaBuildBuilding(&bCan);
			m_pCurrentBuilding->SetCanBeBuilt(bCan);
		}
	}
}

void UComponent_BuildingManager::PlaceCurrentBuilding()
{
	if (m_pCurrentBuilding)
	{
		bool bOut;
		TSubclassOf<AResource_Base> Resource;
		int Amount;
		CnaBuildBuilding(&bOut, &Resource, &Amount);
		if (bOut && !m_pCurrentBuilding->GetOverlapping())
		{
			ABuilding_Base* pCurrentlySeenBuilding = m_pPlayer->GetCurrentlySeenBuilding();
			if (pCurrentlySeenBuilding)
			{
				if (pCurrentlySeenBuilding == m_pCurrentBuilding)
				{
					pCurrentlySeenBuilding->OnLeavePlayerSight();
				}
				else
				{
					m_pPlayer->SetCurrentlySeenBuilding(m_pCurrentBuilding);
				}
			}
			else
			{
				m_pPlayer->SetCurrentlySeenBuilding(m_pCurrentBuilding);
			}

			m_pPlayer->GetPlayerResources()->RemoveResource(Resource, Amount);
			GetWorld()->GetTimerManager().ClearTimer(m_hBuildingTimer);

			m_pCurrentBuilding->OnBuild(Resource);

			StartBuilding();
		}
	}
}


