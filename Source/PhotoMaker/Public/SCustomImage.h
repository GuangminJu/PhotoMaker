// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Images/SImage.h"

/**
 * 
 */

DECLARE_DELEGATE_OneParam(FOnMouseEnterAndMove,const FVector2D&)
DECLARE_DELEGATE_OneParam(FOnMouseWheel,const float)

class PHOTOMAKER_API SCustomImage : public SImage
{
public:
	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	FOnMouseEnterAndMove OnMouseEnterAndMove;
	FOnMouseWheel OnMouseWheelDelegate;
protected:
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	TOptional<FVector2D> MouseLocation;
};
