// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "BuildingSystemEnums.generated.h"


UENUM(BlueprintType)
enum class EStats : uint8
{
	Shield UMETA(DisplayName = "SHIELD"),
	Health UMETA(DisplayName = "HEALTH")
};

UENUM(BlueprintType)
enum class EPlayerStates :uint8
{
	Combat       UMETA(DisplayName = "COMBAT"),
	Construction UMETA(DisplayName = "CONSTRUCTION")
};

UENUM(BlueprintType)
enum class EBuildingStates : uint8
{
	Building UMETA(DisplayName = "BUILDING"),
	Built    UMETA(DisplayName = "BUILT")
};
