// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "BuildingSystemStructure.generated.h"

class UMaterial;

USTRUCT(BlueprintType)
struct FStatValue
{
	GENERATED_BODY()
	
public :
	FStatValue() {};
	FStatValue(int _Min, int _Current, int _Max)
		: MinValue(_Min), CurrentValue(_Current), MaxValue(_Max) {};

public :
	UPROPERTY(EditAnywhere, Category = "FStatValue")
	int MinValue = 0;
	UPROPERTY(EditAnywhere, Category = "FStatValue")
	int CurrentValue = 50;
	UPROPERTY(EditAnywhere, Category = "FStatValue")
	int MaxValue = 100;
};

USTRUCT(BlueprintType)
struct FResourceData
{
	GENERATED_BODY()

public :
	FResourceData() {};
	FResourceData(
		FText _Name, 
		UTexture2D* _pIcon, 
		FLinearColor _GhostBuildingColor,
		UMaterial* _pBuildingMaterial)
		: Name(_Name), 
		  pIcon(_pIcon), 
		  GhostBuildingColor(_GhostBuildingColor), 
		  pBuildingMaterial(_pBuildingMaterial) {};

public :
	UPROPERTY(EditAnywhere, Category = "FResourceData")
	FText Name;
	UPROPERTY(EditAnywhere, Category = "FResourceData")
	UTexture2D* pIcon;
	UPROPERTY(EditAnywhere, Category = "FResourceData")
	FLinearColor GhostBuildingColor;
	UPROPERTY(EditAnywhere, Category = "FResourceData")
	UMaterial* pBuildingMaterial;
};

USTRUCT(BlueprintType)
struct FBuildingData
{
	GENERATED_BODY()
		
public :
	FBuildingData() {};
	FBuildingData(const FText& _Name, const FKey& _HotKey, UTexture2D* const& _pIcon)
		: Name(_Name), HotKey(_HotKey), pIcon(_pIcon) {};

public :
	UPROPERTY(EditAnywhere, Category = "FBuildingData")
	FText Name;
	UPROPERTY(EditAnywhere, Category = "FBuildingData")
	FKey  HotKey;
	UPROPERTY(EditAnywhere, Category = "FBuildingData")
	UTexture2D* pIcon;
};

USTRUCT(BlueprintType)
struct FBuildingResourceVersion
{
	GENERATED_BODY()

public :
	FBuildingResourceVersion() {};
	FBuildingResourceVersion(int _RequireAmount, float _BuildingTime, int _MaximumHealth)
		: RequireAmount(_RequireAmount), BuildingTime(_BuildingTime), MaximumHealth(_MaximumHealth) {};

public :
	UPROPERTY(EditAnywhere, Category = "FBuildingResourceVersion")
	int RequireAmount = 10;
	UPROPERTY(EditAnywhere, Category = "FBuildingResourceVersion")
	float BuildingTime = 2.f;
	UPROPERTY(EditAnywhere, Category = "FBuildingResourceVersion")
	int MaximumHealth = 200;
};
