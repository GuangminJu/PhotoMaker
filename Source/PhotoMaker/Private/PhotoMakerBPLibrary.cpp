// Copyright Epic Games, Inc. All Rights Reserved.

#include "PhotoMakerBPLibrary.h"
#include "PhotoMaker.h"
#include "SStandaloneWindow.h"
#include "WindowSaveConfig.h"
#include "SCustomImage.h"

UPhotoMakerBPLibrary::UPhotoMakerBPLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UTexture2D* UPhotoMakerBPLibrary::CreateWidgetImage(TSharedPtr<SCustomImage>& Image, int32 SrcWidth, int32 SrcHeight,
                                                    const TArray<FColor>& SrcData, UObject* Outer,
                                                    const FString& Name, const EObjectFlags& Flags,
                                                    const FCreateTexture2DParameters& InParams)
{
	return CreateWidgetImage(Image, SrcWidth, SrcHeight, SrcWidth, SrcHeight, SrcData, Outer, Name, Flags, InParams);
}

UTexture2D* UPhotoMakerBPLibrary::CreateWidgetImage(TSharedPtr<SCustomImage>& Image, int32 SrcWidth, int32 SrcHeight,
                                                    int32 ImageWidth, int32 ImageHeight, const TArray<FColor>& SrcData,
                                                    UObject* Outer, const FString& Name,
                                                    const EObjectFlags& Flags,
                                                    const FCreateTexture2DParameters& InParams)
{
	UTexture2D* Texture2D = CreateTexture2D(SrcWidth, SrcHeight, SrcData, Outer, Name, Flags, InParams);

	FSlateBrush* SlateBrush = new FSlateBrush();
	SlateBrush->SetResourceObject(Texture2D);
	SlateBrush->SetImageSize(FVector2D(ImageWidth, ImageHeight));

	SAssignNew(Image, SCustomImage)
		.Image(SlateBrush);

	return Texture2D;
}

UTexture2D* UPhotoMakerBPLibrary::CreateTexture2D(int32 SrcWidth, int32 SrcHeight, const TArray<FColor>& SrcData,
                                                  UObject* Outer, const FString& Name, const EObjectFlags& Flags,
                                                  const FCreateTexture2DParameters& InParams)
{
	UTexture2D * mTexture = UTexture2D::CreateTransient(SrcWidth, SrcHeight, PF_B8G8R8A8);
	mTexture->Filter = TextureFilter::TF_Nearest;

	FColor* Color = (FColor*)(mTexture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE));
	// Lock the texture so it can be modified

	// Create base mip from the pixel color array
	for (int32 y = 0; y < mTexture->PlatformData->Mips[0].SizeY; y++)
	{
		for (int32 x = 0; x < mTexture->PlatformData->Mips[0].SizeX; x++)
		{
			int32 curPixelIndex = (y * mTexture->PlatformData->Mips[0].SizeX) + x;
			Color[curPixelIndex] = SrcData[curPixelIndex];
		}
	}

	//// Unlock the texture
	mTexture->PlatformData->Mips[0].BulkData.Unlock();
	mTexture->UpdateResource();

	return mTexture;
}

void UPhotoMakerBPLibrary::AppendPointOnPhoto(UTexture2D* InTexture, FVector Point, FVector Edge0, FVector Edge1,
                                              FColor Color)
{
	AppendPointOnPhoto(InTexture, FVector2D(Point.X, Point.Y), FVector2D(Edge0.X, Edge0.Y), FVector2D(Edge1.X, Edge1.Y),
	                   Color);
}

void UPhotoMakerBPLibrary::AppendPointOnPhoto(UTexture2D* InTexture, FVector2D Point, FVector2D Edge0, FVector2D Edge1,
                                              FColor Color)
{

	if (!ensureMsgf(InTexture, TEXT("InTexture nullptr")))
		return;

	if (Point < Edge0 || Point > Edge1)
		return;

	FVector2D PointPositionNormalized = (Point - Edge0) / (Edge1 - Edge0);

	FColor* Mip = (FColor*)InTexture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);

	int32 Width = static_cast<uint32_t>(InTexture->GetSurfaceWidth());
	int32 Height = static_cast<uint32_t>(InTexture->GetSurfaceHeight());

	FVector2D PointPosition = FVector2D(Width, Height) * PointPositionNormalized;
	PointPosition = PointPosition.RoundToVector();

	Mip[(int32)PointPosition.Y * InTexture->GetSizeX() + (int32)PointPosition.X] = Color;

	InTexture->PlatformData->Mips[0].BulkData.Unlock();

	InTexture->UpdateResource();

}

void UPhotoMakerBPLibrary::AppendPointOnPhoto(UTexture2D* InTexture, TArray<FVector2D> Points, FVector2D Edge0,
                                              FVector2D Edge1, FColor Color)
{
	if (!ensureMsgf(InTexture, TEXT("InTexture nullptr")))
		return;

	int32 Width = static_cast<uint32_t>(InTexture->GetSurfaceWidth());
	int32 Height = static_cast<uint32_t>(InTexture->GetSurfaceHeight());
	FColor* Mip = (FColor*)InTexture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);

	for (const FVector2D& Point : Points)
	{
		if (Point.X < Edge0.X || Point.Y < Edge0.Y || Point.X > Edge1.X || Point.Y > Edge1.Y)
			continue;

		FVector2D PointPositionNormalized = (Point - Edge0) / (Edge1 - Edge0);

		if (PointPositionNormalized.X > 1.0 || PointPositionNormalized.Y > 1.0)
			continue;

		FVector2D PointPosition = FVector2D(Width, Height) * PointPositionNormalized;
		PointPosition = PointPosition.RoundToVector();
		Mip[(int32)PointPosition.Y * InTexture->GetSizeX() + (int32)PointPosition.X] = Color;
	}

	InTexture->PlatformData->Mips[0].BulkData.Unlock();

	InTexture->UpdateResource();
}

void UPhotoMakerBPLibrary::AppendPointOnPhoto(UTexture2D* InTexture, TArray<FVector_NetQuantize> Points,
                                              FVector2D Edge0, FVector2D Edge1, FColor Color)
{
	if (!ensureMsgf(InTexture, TEXT("InTexture nullptr")))
		return;

	int32 Width = InTexture->PlatformData->Mips[0].SizeX;
	int32 Height = InTexture->PlatformData->Mips[0].SizeY;
	
	FColor* Mip = (FColor*)InTexture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);

	for (const FVector_NetQuantize& Point3D : Points)
	{
		FVector2D Point(Point3D.X, Point3D.Y);
		if (Point.X < Edge0.X || Point.Y < Edge0.Y || Point.X > Edge1.X || Point.Y > Edge1.Y)
			continue;

		FVector2D PointPositionNormalized = (Point - Edge0) / (Edge1 - Edge0);

		if (PointPositionNormalized.X > 1.0 || PointPositionNormalized.Y > 1.0)
			continue;

		FVector2D PointPosition = FVector2D(Width, Height) * PointPositionNormalized;
		PointPosition = PointPosition.RoundToVector();
		Mip[(int32)PointPosition.Y * InTexture->GetSizeX() + (int32)PointPosition.X] = Color;
	}

	InTexture->PlatformData->Mips[0].BulkData.Unlock();

	InTexture->UpdateResource();
}

void UPhotoMakerBPLibrary::SetPhotoOneColor(UTexture2D* InTexture, FColor Color)
{
	int32 Width = static_cast<uint32_t>(InTexture->GetSurfaceWidth());
	int32 Height = static_cast<uint32_t>(InTexture->GetSurfaceHeight());

	FColor* Mip = (FColor*)InTexture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);

	for (int32 y = 0; y < Height; y++)
	{
		for (int32 x = 0; x < Width; x++)
		{
			Mip[Height * y + x] = Color;
		}
	}

	InTexture->PlatformData->Mips[0].BulkData.Unlock();
	InTexture->UpdateResource();
}

TSharedPtr<SStandaloneWindow> UPhotoMakerBPLibrary::CreateStandaloneWindow()
{
	UWindowSaveConfig* WindowSaveConfig = UWindowSaveConfig::GetWindowSaveConfig();
	WindowSaveConfig->LoadConfig();

	TSharedPtr<SStandaloneWindow> Window = SNew(SStandaloneWindow)
		.Title(NSLOCTEXT("RL Info", "ReinforcementLearning", "Reinforcement Learning"))
		.AutoCenter(EAutoCenter::None)
		.ClientSize(WindowSaveConfig->ClientSize)
		.ScreenPosition(WindowSaveConfig->ScreenPosition)
		.IsTopmostWindow(true);

	FSlateApplication::Get().AddWindow(Window.ToSharedRef(), true);

	Window->SetConfig(WindowSaveConfig);

	return Window;
}
