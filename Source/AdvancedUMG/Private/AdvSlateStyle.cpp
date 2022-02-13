// Copyright (c) 2022 cathery

#include "AdvSlateStyle.h"

#include "Slate/AdvButtonGenericStyle.h"
#include "Slate/AdvButtonSimpleStyle.h"
#include "Styling/SlateStyleRegistry.h"

TSharedPtr<ISlateStyle> FAdvSlateStyle::Instance = nullptr;

FAdvSlateStyle::FAdvSlateStyle(const FName& InStyleSetName): FSlateStyleSet(InStyleSetName)
{
	const FAdvButtonGenericStyle GenericButtonStyle = FAdvButtonGenericStyle()
		.SetNormal(FSlateBoxBrush(FPaths::EngineContentDir() / "Slate/Common/Button.png", FVector2D(32, 32), 8.0f / 32.0f))
		.SetHovered(FSlateBoxBrush(FPaths::EngineContentDir() / "Slate/Common/Button_Hovered.png", FVector2D(32, 32), 8.0f / 32.0f))
		.SetPressed(FSlateBoxBrush(FPaths::EngineContentDir() / "Slate/Common/Button_Pressed.png", FVector2D(32, 32), 8.0f / 32.0f))
		.SetNormalPadding(FMargin(2, 2, 2, 2))
		.SetPressedPadding(FMargin(2, 3, 2, 1));

	Set("DefaultGenericButton", GenericButtonStyle);

	const FAdvButtonSimpleStyle SimpleButtonStyle = FAdvButtonSimpleStyle()
		.SetImage(FSlateBoxBrush( FPaths::EngineContentDir() / "Slate/Common/Button.png", FVector2D(32,32), 8.0f/32.0f ))
		.SetDownNudge(5.0f)
		.SetPadding(FMargin());

	Set("DefaultSimpleButton", SimpleButtonStyle);
}

void FAdvSlateStyle::Initialize()
{
	Instance = MakeShareable(new FAdvSlateStyle("AdvSlateStyle"));

	FSlateStyleRegistry::RegisterSlateStyle(*Instance.Get());
}

void FAdvSlateStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*Instance.Get());

	Instance.Reset();
}
