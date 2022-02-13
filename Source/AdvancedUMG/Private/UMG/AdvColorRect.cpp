// Copyright (c) 2022 cathery

#include "UMG/AdvColorRect.h"

#include "Widgets/DeclarativeSyntaxSupport.h"

#include "Slate/SAdvColorRect.h"

UAdvColorRect::UAdvColorRect()
	: ColorAndOpacity(FLinearColor::White)
	, OutlineColorAndOpacity(FLinearColor::Black)
{
	bIsVariable = true;
	Visibility  = ESlateVisibility::Visible;
}

void UAdvColorRect::SetColorAndOpacity(FLinearColor InColorAndOpacity)
{
	ColorAndOpacity = InColorAndOpacity;
	if (MyRect)
	{
		MyRect->SetColorAndOpacity(InColorAndOpacity);
	}
}

void UAdvColorRect::SetOpacity(float InOpacity)
{
	ColorAndOpacity.A = InOpacity;
	if (MyRect)
	{
		MyRect->SetColorAndOpacity(ColorAndOpacity);
	}
}

void UAdvColorRect::SetOutlineColorAndOpacity(FLinearColor InOutlineColorAndOpacity)
{
	OutlineColorAndOpacity = InOutlineColorAndOpacity;
	if (MyRect)
	{
		MyRect->SetOutlineColorAndOpacity(InOutlineColorAndOpacity);
	}
}

void UAdvColorRect::SetOutlineOpacity(float InOutlineOpacity)
{
	OutlineColorAndOpacity.A = InOutlineOpacity;
	if (MyRect)
	{
		MyRect->SetOutlineColorAndOpacity(OutlineColorAndOpacity);
	}
}

void UAdvColorRect::SetOutlineThickness(const FMargin& InOutlineThickness)
{
	OutlineThickness = InOutlineThickness;
	if (MyRect)
	{
		MyRect->SetOutlineThickness(InOutlineThickness);
	}
}

void UAdvColorRect::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	const TAttribute<FSlateColor> ColorAndOpacityBinding        = PROPERTY_BINDING(FSlateColor, ColorAndOpacity);
	const TAttribute<FSlateColor> OutlineColorAndOpacityBinding = PROPERTY_BINDING(FSlateColor, OutlineColorAndOpacity);

	if (MyRect)
	{
		MyRect->SetColorAndOpacity(ColorAndOpacityBinding);
		MyRect->SetOutlineColorAndOpacity(OutlineColorAndOpacityBinding);
		MyRect->SetOutlineThickness(OutlineThickness);
		MyRect->SetOnMouseButtonDown(BIND_UOBJECT_DELEGATE(FPointerEventHandler, HandleMouseButtonDown));
	}
}

void UAdvColorRect::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyRect.Reset();
}

FReply UAdvColorRect::HandleMouseButtonDown(const FGeometry& Geometry, const FPointerEvent& MouseEvent) const
{
	if (OnMouseButtonDownEvent.IsBound())
	{
		return OnMouseButtonDownEvent.Execute(Geometry, MouseEvent).NativeReply;
	}

	return FReply::Unhandled();
}

TSharedPtr<SAdvPanel> UAdvColorRect::GetSlateAdvPanel() const
{
	return MyRect;
}

void UAdvColorRect::MakeSlateWidget()
{
	MyRect = SNew(SAdvColorRect);
}
