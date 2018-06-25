// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "../Widgets/Widget_Main.h"
#include "../Components/Component_PlayerStats.h"
#include "../Components/Component_PlayerResource.h"
#include "../Components/Component_BuildingManager.h"
#include "../Buildings/Building_Base.h"

#include <Components/InputComponent.h>
#include <Components/Image.h>
#include <Components/HorizontalBox.h>
#include <GameFramework/Controller.h>
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <UObject/ConstructorHelpers.h>
#include <GameFramework/PlayerController.h>
#include <TimerManager.h>


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	m_pSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	m_pSpringArm->bDoCollisionTest = false;
	m_pSpringArm->bUsePawnControlRotation = true;
	m_pSpringArm->bInheritPitch = true;
	m_pSpringArm->bInheritYaw = true;
	m_pSpringArm->bInheritRoll = false;
	m_pSpringArm->SetupAttachment(RootComponent);

	m_pCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	m_pCamera->SetupAttachment(m_pSpringArm);

	m_pPlayerStats = CreateDefaultSubobject<UComponent_PlayerStats>(TEXT("PlayerStats"));
	m_pPlayerResources = CreateDefaultSubobject<UComponent_PlayerResource>(TEXT("PlayerResources"));
	m_pBuildingManager = CreateDefaultSubobject<UComponent_BuildingManager>(TEXT("BuildingManager"));

	static ConstructorHelpers::FClassFinder<UUserWidget> WB_Main(TEXT("WidgetBlueprint'/Game/BuildingSystem/Blueprints/Widgets/WB_Main.WB_Main_C'"));
	if (WB_Main.Succeeded())
	{
		m_MainWidgetClass = WB_Main.Class;
	}
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (m_MainWidgetClass)
	{
		m_pMainWidget = CreateWidget<UWidget_Main>(GWorld, m_MainWidgetClass);

		m_State = EPlayerStates::Combat;
		UpdateStateDisplay();

		m_pPlayerStats->InitializeManager(this);
		m_pPlayerResources->InitializeManager(this);
		m_pBuildingManager->Initialize(this);

		m_pMainWidget->AddToViewport();

		GetWorldTimerManager().SetTimer(m_hSightTimer, this, &APlayerCharacter::SightTrace, m_SightTraceTickRate, true);
	}	
}


// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::_MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::_MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::_LookUp);
	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::_Turn);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::_Jump);
	PlayerInputComponent->BindKey(EKeys::Tab, IE_Pressed, this, &APlayerCharacter::_Tab);
	PlayerInputComponent->BindKey(EKeys::Q, IE_Pressed, this, &APlayerCharacter::_QKey);
	PlayerInputComponent->BindKey(EKeys::LeftMouseButton, IE_Pressed, this, &APlayerCharacter::_LeftMouseButton);

	PlayerInputComponent->BindKey(EKeys::AnyKey, IE_Pressed, this, &APlayerCharacter::_AnyKey);
}

void APlayerCharacter::UpdateStateDisplay()
{
	UTexture2D* pCrosshairTex = nullptr;
	UTexture2D* pStateIconTex = nullptr;

	if (m_State == EPlayerStates::Combat)
	{
		m_pBuildingManager->OnStopBuilding();

		pCrosshairTex = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/Game/BuildingSystem/Resources/Textures/Crosshairs/Crosshair_Combat.Crosshair_Combat'"));
		pStateIconTex = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/Game/BuildingSystem/Resources/Textures/States/Icon_Combat.Icon_Combat'"));
	
		m_pMainWidget->GetCrosshair()->SetBrushFromTexture(pCrosshairTex, true);
		m_pMainWidget->GetStateIcon()->SetBrushFromTexture(pStateIconTex, true);
		m_pMainWidget->GetBuildingBox()->SetRenderOpacity(0.8f);
		m_pMainWidget->GetBuildingBox()->SetRenderScale(FVector2D(0.8f, 0.8f));

		m_pMainWidget->ShowCurrentResource(false);
	}
	else
	{
		m_pBuildingManager->SelectCurrentBuilding(true);

		pCrosshairTex = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/Game/BuildingSystem/Resources/Textures/Crosshairs/Crosshair_Construction.Crosshair_Construction'"));
		pStateIconTex = LoadObject<UTexture2D>(nullptr, TEXT("Texture2D'/Game/BuildingSystem/Resources/Textures/States/Icon_Construction.Icon_Construction'"));
	
		m_pMainWidget->GetCrosshair()->SetBrushFromTexture(pCrosshairTex, true);
		m_pMainWidget->GetStateIcon()->SetBrushFromTexture(pStateIconTex, true);
		m_pMainWidget->GetBuildingBox()->SetRenderOpacity(1.f);
		m_pMainWidget->GetBuildingBox()->SetRenderScale(FVector2D(1.f, 1.f));

		m_pMainWidget->UpdateCurrentResource(m_pPlayerResources);
		m_pMainWidget->ShowCurrentResource(true);
	}
}

void APlayerCharacter::ChangeState(EPlayerStates _NewState)
{
	if (m_State != _NewState)
	{
		m_State = _NewState;
		UpdateStateDisplay();
	}
}

void APlayerCharacter::SightTrace()
{
	FHitResult Result;
	int ForwardOffset = m_pBuildingManager->GetForwardBuildingOffset() * 3;
	FVector LookAt    = m_pBuildingManager->GetPlayerLookAtLocation(ForwardOffset);
	FVector ToGridLoc = m_pBuildingManager->ToGridLocation(LookAt);
	FVector Start     = m_pBuildingManager->GetLocationOffset() + GetActorLocation();	
	FVector End       = m_pBuildingManager->GetLocationOffset() + ToGridLoc;

	bool bHit = GetWorld()->LineTraceSingleByChannel(Result, Start, End, ECollisionChannel::ECC_Visibility);
	if (bHit)
	{
		ABuilding_Base* pBuildingObj = Cast<ABuilding_Base>(Result.GetActor());
		if (pBuildingObj)
		{
			if (m_pCurrentlySeenBuilding)
			{
				if (m_pCurrentlySeenBuilding != pBuildingObj)
				{
					m_pCurrentlySeenBuilding->OnLeavePlayerSight();
					m_pCurrentlySeenBuilding = pBuildingObj;
					m_pCurrentlySeenBuilding->OnEnterPlayerSight();
				}
			}
			else
			{
				m_pCurrentlySeenBuilding = pBuildingObj;
				m_pCurrentlySeenBuilding->OnEnterPlayerSight();
			}
		}
		else
		{
			if (m_pCurrentlySeenBuilding)
			{
				m_pCurrentlySeenBuilding->OnLeavePlayerSight();
				m_pCurrentlySeenBuilding = nullptr;
			}
		}
	}
}

void APlayerCharacter::_MoveForward(float _Value)
{
	if (_Value != 0.f)
	{
		FVector Forward = GetActorForwardVector();
		AddMovementInput(Forward, _Value);
	}
}

void APlayerCharacter::_MoveRight(float _Value)
{
	if (_Value != 0.f)
	{
		FVector Right = GetActorRightVector();
		AddMovementInput(Right, _Value);
	}
}

void APlayerCharacter::_LookUp(float _Value)
{
	if (_Value != 0.f)
	{
		FRotator Rot = GetControlRotation();
		Rot.Pitch = FMath::ClampAngle(Rot.Pitch + _Value, -45.f, 90.f);

		GetController()->SetControlRotation(Rot);
	}
}

void APlayerCharacter::_Turn(float _Value)
{
	if (_Value != 0.f)
	{
		FRotator Rot = GetControlRotation();
		Rot.Yaw += _Value;

		GetController()->SetControlRotation(Rot);
	}	
}

void APlayerCharacter::_Jump()
{
	Jump();
}

void APlayerCharacter::_Tab()
{
	if (m_State == EPlayerStates::Construction)
	{
		m_pPlayerResources->SelectNextResource();
	}
}

void APlayerCharacter::_QKey()
{	
	ChangeState(EPlayerStates(((int)m_State + 1) % 2));
}

void APlayerCharacter::_LeftMouseButton()
{
	if (m_State == EPlayerStates::Construction)
	{
		m_pBuildingManager->PlaceCurrentBuilding();
	}
}

void APlayerCharacter::_AnyKey()
{
	APlayerController* pController = Cast<APlayerController>(GetController());
	
	const TArray<TSubclassOf<ABuilding_Base>> BuildingClasses = m_pBuildingManager->GetBuildingClasses();

	for(int i = 0; i < BuildingClasses.Num(); ++i )
	{
		FKey HotKey = BuildingClasses[i].GetDefaultObject()->GetBuildingData().HotKey;
		if (pController->WasInputKeyJustPressed(HotKey))
		{
			m_pBuildingManager->SelectBuildingByIndex(i);
			return;
		}
	}
	
}



