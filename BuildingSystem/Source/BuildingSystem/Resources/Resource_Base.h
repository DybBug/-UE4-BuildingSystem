// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../BuildingSystemStructure.h"
#include "Resource_Base.generated.h"

UCLASS()
class BUILDINGSYSTEM_API AResource_Base : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AResource_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public :
	const FResourceData& GetResourceData() const { return m_ResourceData; }


protected :
	UPROPERTY(EditAnywhere, Category = "AResource_Base|Data")
	FResourceData m_ResourceData;
	
};
