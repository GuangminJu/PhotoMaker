// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object.h"
#include "Engine/NetSerialization.h"
#include "PointStructure.generated.h"

/**
 * 
 */

USTRUCT()
struct FPointStructure
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FVector_NetQuantize> Vectors;

	UPROPERTY()
	FColor Color;
};
