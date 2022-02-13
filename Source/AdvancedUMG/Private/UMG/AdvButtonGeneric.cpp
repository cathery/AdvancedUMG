// Copyright (c) 2022 cathery

#include "UMG/AdvButtonGeneric.h"

#include "Widgets/DeclarativeSyntaxSupport.h"

#include "Slate/SAdvButtonGeneric.h"

UAdvButtonGeneric::UAdvButtonGeneric()
	: Style(FAdvSlateStyle::Get().GetWidgetStyle<FAdvButtonGenericStyle>("DefaultGenericButton"))
	, ColorAndOpacity(FLinearColor::White)
	, BackgroundColor(FLinearColor::White)
{
}

void UAdvButtonGeneric::SetStyle(const FAdvButtonGenericStyle& InStyle)
{
	Style = InStyle;
	if (MyButton)
	{
		MyButton->SetButtonStyle(&Style);
	}
}

void UAdvButtonGeneric::SetColorAndOpacity(FLinearColor InColorAndOpacity)
{
	ColorAndOpacity = InColorAndOpacity;
	if (MyButton)
	{
		MyButton->SetColorAndOpacity(InColorAndOpacity);
	}
}

void UAdvButtonGeneric::SetBackgroundColor(FLinearColor InBackgroundColor)
{
	BackgroundColor = InBackgroundColor;
	if (MyButton)
	{
		MyButton->SetBorderBackgroundColor(InBackgroundColor);
	}
}

void UAdvButtonGeneric::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (MyButton)
	{
		MyButton->SetButtonStyle(&Style);
		MyButton->SetColorAndOpacity(ColorAndOpacity);
		MyButton->SetBorderBackgroundColor(BackgroundColor);
	}
}

void UAdvButtonGeneric::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyButton.Reset();
}

void UAdvButtonGeneric::MakeSlateWidget()
{
	MyButton = SNew(SAdvButtonGeneric)
		.ClickMethod(ClickMethod)
		.TouchMethod(TouchMethod)
		.PressMethod(PressMethod)
		.IsFocusable(IsFocusable)
		.OnClicked_UObject(this, &ThisClass::SlateHandleClicked)
		.OnPressed_UObject(this, &ThisClass::SlateHandlePressed)
		.OnReleased_UObject(this, &ThisClass::SlateHandleReleased)
		.OnHovered_UObject(this, &ThisClass::SlateHandleHovered)
		.OnUnhovered_UObject(this, &ThisClass::SlateHandleUnhovered)
		.Style(&Style)
		.ColorAndOpacity(ColorAndOpacity)
		.BackgroundColor(BackgroundColor);
}

TSharedPtr<SAdvButtonBase> UAdvButtonGeneric::GetSlateAdvButton() const
{
	return MyButton;
}
