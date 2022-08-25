// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "WindowSaveConfig.generated.h"

/**
 * 
 */
UCLASS(Config=Game)
class PHOTOMAKER_API UWindowSaveConfig : public UObject
{
	GENERATED_BODY()
	
public:
	UWindowSaveConfig()
		:ScreenPosition(FVector2D(0.,0.))
	{
		
	}
	
	static UWindowSaveConfig* GetWindowSaveConfig()
	{
		return GetMutableDefault<UWindowSaveConfig>();
	}

	UPROPERTY(Config)
	FVector2D ClientSize;
	
	UPROPERTY(Config)
	FVector2D ScreenPosition;
};
