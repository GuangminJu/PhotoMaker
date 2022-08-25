// Fill out your copyright notice in the Description page of Project Settings.


#include "SStandaloneWindow.h"
#include "Widgets/Layout/SGridPanel.h"
#include "WindowSaveConfig.h"

void SStandaloneWindow::Construct(const FArguments& InArgs)
{
	SAssignNew(GridPanel, SGridPanel);
	
	SWindow::Construct(InArgs);
	(*ContentSlot)
	[
		GridPanel.ToSharedRef()
	];
}

void SStandaloneWindow::SetConfig(UWindowSaveConfig* InWindowSaveConfig)
{
	WindowSaveConfig = InWindowSaveConfig;

	OnWindowMoved.BindSP(this, &SStandaloneWindow::OnWindowMovedCall);
}

void SStandaloneWindow::AddWidget(TSharedRef<SWidget> InWidget, int32 Column, int32 Row)
{
	GridPanel->AddSlot(Column, Row)
	[
		InWidget
	];
}

void SStandaloneWindow::OnWindowMovedCall(const TSharedRef<SWindow>& Window)
{
	WindowSaveConfig->ScreenPosition = Window->GetPositionInScreen();
	WindowSaveConfig->SaveConfig();
}
