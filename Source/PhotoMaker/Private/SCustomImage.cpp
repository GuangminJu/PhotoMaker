// Fill out your copyright notice in the Description page of Project Settings.


#include "SCustomImage.h"

#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SCustomImage::Construct(const FArguments& InArgs)
{
	SImage::Construct(InArgs);
	/*
	ChildSlot
	[
		// Populate the widget
	];
	*/
}

FReply SCustomImage::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	MouseLocation = MouseEvent.GetScreenSpacePosition();

	return SImage::OnMouseButtonDown(MyGeometry, MouseEvent);
}

FReply SCustomImage::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	FReply Reply = SImage::OnMouseMove(MyGeometry, MouseEvent);

	if (MouseLocation.IsSet())
	{
		FVector2D Delta = MouseLocation.GetValue() - MouseEvent.GetScreenSpacePosition();
		MouseLocation = MouseEvent.GetScreenSpacePosition();
		OnMouseEnterAndMove.Execute(Delta);
	}

	
	return Reply;
}

FReply SCustomImage::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	MouseLocation.Reset();

	return SImage::OnMouseButtonUp(MyGeometry, MouseEvent);
}

FReply SCustomImage::OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	float WheelDelta = MouseEvent.GetWheelDelta();
	OnMouseWheelDelegate.Execute(WheelDelta);
	return SImage::OnMouseWheel(MyGeometry, MouseEvent);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
