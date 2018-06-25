// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"


enum class EPlayerStates : uint8;
class USpringArmComponent;
class UCameraComponent;
class UWidget_Main;
class UComponent_PlayerStats;
class UComponent_PlayerResource;
class UComponent_BuildingManager;
class ABuilding_Base;

UCLASS()
class BUILDINGSYSTEM_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public :
	void UpdateStateDisplay();
	void ChangeState(EPlayerStates _NewState);

	void SightTrace();

	/* Get */
	const EPlayerStates GetState() const { return m_State; }

	UWidget_Main*               const& GetMainWidget()            const { return m_pMainWidget; }
	UComponent_PlayerResource*  const& GetPlayerResources()       const { return m_pPlayerResources; }
	UComponent_BuildingManager* const& GetBuildingManager()       const { return m_pBuildingManager; }
	UCameraComponent*           const& GetCamera()                const { return m_pCamera; }
	ABuilding_Base*             const& GetCurrentlySeenBuilding() const { return m_pCurrentlySeenBuilding; }

	/* Set */
	void SetCurrentlySeenBuilding(ABuilding_Base* const& _pBuildingObj) { m_pCurrentlySeenBuilding = _pBuildingObj; }

private :
	UFUNCTION()
	void _MoveForward(float _Value);
	UFUNCTION()
	void _MoveRight(float _Value);
	UFUNCTION()
	void _LookUp(float _Value);
	UFUNCTION()
	void _Turn(float _Value);

	UFUNCTION()
	void _Jump();
	UFUNCTION()
	void _Tab();
	UFUNCTION()
	void _QKey();
	UFUNCTION()
	void _LeftMouseButton();
	
	UFUNCTION()
	void _AnyKey();

protected :
	UPROPERTY(VisibleAnywhere, Category = "APlayerCharacter|Components")
	USpringArmComponent* m_pSpringArm;

	UPROPERTY(VisibleAnywhere, Category = "APlayerCharacter|Components")
	UCameraComponent* m_pCamera;

	UPROPERTY(VisibleAnywhere, Category = "APlayerCharacter|Components")
	UComponent_PlayerStats* m_pPlayerStats;

	UPROPERTY(VisibleAnywhere, Category = "APlayerCharacter|Components")
	UComponent_PlayerResource* m_pPlayerResources;

	UPROPERTY(VisibleAnywhere, Category = "APlayerCharacter|Components")
	UComponent_BuildingManager* m_pBuildingManager;

	UPROPERTY(VisibleInstanceOnly, Category = "APlayerCharacter")
	EPlayerStates m_State;

	TSubclassOf<UWidget_Main> m_MainWidgetClass;
	UWidget_Main* m_pMainWidget;

	ABuilding_Base* m_pCurrentlySeenBuilding;

	float m_SightTraceTickRate = 0.05f;
	FTimerHandle m_hSightTimer;
	
};
