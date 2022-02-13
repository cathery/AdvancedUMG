// Copyright (c) 2022 cathery

#include "UMG/AdvButtonSimple.h"

#include "Widgets/DeclarativeSyntaxSupport.h"

#include "Slate/SAdvButtonSimple.h"

UAdvButtonSimple::UAdvButtonSimple()
	: Style(FAdvSlateStyle::Get().GetWidgetStyle<FAdvButtonSimpleStyle>("DefaultSimpleButton"))
{
}

void UAdvButtonSimple::SetStyle(const FAdvButtonSimpleStyle& InStyle)
{
	Style = InStyle;
	if (MyButton)
	{
		MyButton->SetStyle(&Style);
	}
}

void UAdvButtonSimple::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (MyButton)
	{
		MyButton->SetStyle(&Style);
	}
}

void UAdvButtonSimple::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyButton.Reset();
}

void UAdvButtonSimple::MakeSlateWidget()
{
	MyButton = SNew(SAdvButtonSimple)
		.ClickMethod(ClickMethod)
		.TouchMethod(TouchMethod)
		.PressMethod(PressMethod)
		.IsFocusable(IsFocusable)
		.OnClicked_UObject(this, &ThisClass::SlateHandleClicked)
		.OnPressed_UObject(this, &ThisClass::SlateHandlePressed)
		.OnReleased_UObject(this, &ThisClass::SlateHandleReleased)
		.OnHovered_UObject(this, &ThisClass::SlateHandleHovered)
		.OnUnhovered_UObject(this, &ThisClass::SlateHandleUnhovered)
		.Style(&Style);
}

TSharedPtr<SAdvButtonBase> UAdvButtonSimple::GetSlateAdvButton() const
{
	return MyButton;
}
