// Copyright (c) 2022 cathery

#include "Slate/SAdvColorRect.h"

#include "Layout/LayoutUtils.h"

const FSlateBrush SAdvColorRect::Brush = FSlateBrush();

SAdvColorRect::SAdvColorRect()
	: ColorAndOpacity(FLinearColor::White)
	, OutlineColorAndOpacity(FLinearColor::Black)
	, OutlineThickness(0.0f)
{
}

void SAdvColorRect::Construct(const FArguments& InArgs)
{
	// Call the parent constructor with our slots
	SAdvPanel::FArguments ParentArgs;
	ParentArgs.Slots = InArgs.Slots;
	SAdvPanel::Construct(ParentArgs);

	ColorAndOpacity        = InArgs._ColorAndOpacity;
	OutlineColorAndOpacity = InArgs._OutlineColorAndOpacity;
	OutlineThickness       = InArgs._OutlineThickness;
	SetOnMouseButtonDown(InArgs._OnMouseButtonDown);
}

void SAdvColorRect::SetColorAndOpacity(const TAttribute<FSlateColor>& InColorAndOpacity)
{
	SetAttribute(ColorAndOpacity, InColorAndOpacity, EInvalidateWidgetReason::Paint);
}

void SAdvColorRect::SetColorAndOpacity(FLinearColor InColorAndOpacity)
{
	SetColorAndOpacity(TAttribute<FSlateColor>(InColorAndOpacity));
}

void SAdvColorRect::SetOutlineColorAndOpacity(const TAttribute<FSlateColor>& InOutlineColorAndOpacity)
{
	SetAttribute(OutlineColorAndOpacity, InOutlineColorAndOpacity, EInvalidateWidgetReason::Paint);
}

void SAdvColorRect::SetOutlineColorAndOpacity(FLinearColor InOutlineColorAndOpacity)
{
	SetOutlineColorAndOpacity(TAttribute<FSlateColor>(InOutlineColorAndOpacity));
}

void SAdvColorRect::SetOutlineThickness(const TAttribute<FMargin> InOutlineThickness)
{
	SetAttribute(OutlineThickness, InOutlineThickness, EInvalidateWidgetReason::Paint);
}

int32 SAdvColorRect::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	int32 MaxLayerId = LayerId;

	const bool             bIsEnabled  = ShouldBeEnabled(bParentEnabled);
	const ESlateDrawEffect DrawEffects = bIsEnabled ? ESlateDrawEffect::None : ESlateDrawEffect::DisabledEffect;

	const FLinearColor FinalColorAndOpacity(InWidgetStyle.GetColorAndOpacityTint() * ColorAndOpacity.Get().GetColor(InWidgetStyle));

	FSlateDrawElement::MakeBox(OutDrawElements, MaxLayerId++, AllottedGeometry.ToPaintGeometry(), &Brush, DrawEffects, FinalColorAndOpacity);

	MaxLayerId = PaintChildren(Args, AllottedGeometry, MyCullingRect, OutDrawElements, MaxLayerId, InWidgetStyle, bIsEnabled);

	const float TotalOutlineThickness = OutlineThickness.Get().GetTotalSpaceAlong<Orient_Horizontal>() + OutlineThickness.Get().GetTotalSpaceAlong<Orient_Vertical>();

	if (TotalOutlineThickness > 0.0f)
	{
		++MaxLayerId;
		const FLinearColor FinalOutlineColorAndOpacity(InWidgetStyle.GetColorAndOpacityTint() * OutlineColorAndOpacity.Get().GetColor(InWidgetStyle));

		// We will manually draw each border as a box
		// Because none of the border/lines functionality in ue4 seems to work like we need it to

		const FVector2D& LocalSize     = AllottedGeometry.GetLocalSize();
		const FMargin&      LineThickness = OutlineThickness.Get();

		const FGeometry& TopLineGeometry = AllottedGeometry.MakeChild(
			FVector2D(LocalSize.X, LineThickness.Top),
			FSlateLayoutTransform(FVector2D::ZeroVector));

		const FGeometry& RightLineGeometry = AllottedGeometry.MakeChild(
			FVector2D(LineThickness.Right, LocalSize.Y - LineThickness.Top - LineThickness.Bottom),
			FSlateLayoutTransform(FVector2D(LocalSize.X - LineThickness.Right, LineThickness.Top)));

		const FGeometry& BottomLineGeometry = AllottedGeometry.MakeChild(
			FVector2D(LocalSize.X, LineThickness.Bottom),
			FSlateLayoutTransform(FVector2D(0.0f, LocalSize.Y - LineThickness.Bottom)));

		const FGeometry& LeftLineGeometry = AllottedGeometry.MakeChild(
			FVector2D(LineThickness.Left, LocalSize.Y - LineThickness.Top - LineThickness.Bottom),
			FSlateLayoutTransform(FVector2D(0.0f, LineThickness.Top)));

		FSlateDrawElement::MakeBox(OutDrawElements, MaxLayerId, TopLineGeometry.ToPaintGeometry(), &Brush, DrawEffects, FinalOutlineColorAndOpacity);
		FSlateDrawElement::MakeBox(OutDrawElements, MaxLayerId, RightLineGeometry.ToPaintGeometry(), &Brush, DrawEffects, FinalOutlineColorAndOpacity);
		FSlateDrawElement::MakeBox(OutDrawElements, MaxLayerId, BottomLineGeometry.ToPaintGeometry(), &Brush, DrawEffects, FinalOutlineColorAndOpacity);
		FSlateDrawElement::MakeBox(OutDrawElements, MaxLayerId, LeftLineGeometry.ToPaintGeometry(), &Brush, DrawEffects, FinalOutlineColorAndOpacity);
	}

	return MaxLayerId;
}

FVector2D SAdvColorRect::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
	const FVector2D ContentSize = SAdvPanel::ComputeDesiredSize(LayoutScaleMultiplier);

	// idk if anything should be done here yet

	return ContentSize;
}
