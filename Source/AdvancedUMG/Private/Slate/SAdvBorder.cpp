// Copyright (c) 2022 cathery

#include "Slate/SAdvBorder.h"

#include "Layout/LayoutUtils.h"

SAdvBorder::SAdvBorder()
	: ImageScale(1.0f)
{
	Image.DrawAs = ESlateBrushDrawType::Border;
}

void SAdvBorder::Construct(const FArguments& InArgs)
{
	// Call the parent constructor with our slots
	SAdvPanel::FArguments ParentArgs;
	ParentArgs.Slots = InArgs.Slots;
	SAdvPanel::Construct(ParentArgs);

	Image = InArgs._Image;
	Image.DrawAs = ESlateBrushDrawType::Border;

	ImageScale = InArgs._ImageScale;
}
void SAdvBorder::SetImage(const FSlateBrush& InImage)
{
	Image = InImage;
	Image.DrawAs = ESlateBrushDrawType::Border;
}

void SAdvBorder::SetImageScale(float InImageScale)
{
	ImageScale = InImageScale;
}

int32 SAdvBorder::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	int32 MaxLayerId = LayerId;

	MaxLayerId = PaintChildren(Args, AllottedGeometry, MyCullingRect, OutDrawElements, MaxLayerId, InWidgetStyle, ShouldBeEnabled(bParentEnabled));

	// Image.DrawAs = ESlateBrushDrawType::Border;
	const FLinearColor FinalColorAndOpacity(InWidgetStyle.GetColorAndOpacityTint() * Image.GetTint(InWidgetStyle));
	const FPaintGeometry& PaintGeometry = FPaintGeometry(
			FSlateLayoutTransform(
				ImageScale,
				AllottedGeometry.GetAccumulatedLayoutTransform().GetTranslation()
			),
			AllottedGeometry.GetAccumulatedRenderTransform(),
			AllottedGeometry.GetLocalSize(),
			AllottedGeometry.HasRenderTransform());
	FSlateDrawElement::MakeBox(OutDrawElements, ++MaxLayerId, PaintGeometry, &Image, ESlateDrawEffect::None, FinalColorAndOpacity);

	return MaxLayerId;
}

FVector2D SAdvBorder::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
	// Return the size of children or the size of the image, whichever is bigger
	const FVector2D& ContentSize = SAdvPanel::ComputeDesiredSize(LayoutScaleMultiplier);
	const FVector2D& ImageSize = Image.ImageSize;

	const FVector2D MaxSize(
		FMath::Max(ContentSize.X, ImageSize.X),
		FMath::Max(ContentSize.Y, ImageSize.Y)
	);
	return MaxSize;
}
