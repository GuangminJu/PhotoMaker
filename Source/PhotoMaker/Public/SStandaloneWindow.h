// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class UWindowSaveConfig;
class SGridPanel;

class SStandaloneWindow : public SWindow
{
public:
	void Construct(const FArguments& InArgs);
	
	SStandaloneWindow():
		WindowSaveConfig(nullptr)
	{
		
	}
	virtual void SetConfig(UWindowSaveConfig* InWindowSaveConfig);
	virtual void AddWidget(TSharedRef<SWidget> InWidget, int32 Column, int32 Row);
protected:
	void OnWindowMovedCall(const TSharedRef<SWindow>& Window);

	UWindowSaveConfig* WindowSaveConfig;

	TSharedPtr<SGridPanel> GridPanel;
};
