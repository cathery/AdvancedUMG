// Copyright (c) 2022 cathery

#include "Slate/SAdvImage.h"

#include "Layout/LayoutUtils.h"

SLATE_IMPLEMENT_WIDGET(SAdvImage)
void SAdvImage::PrivateRegisterAttributes(FSlateAttributeInitializer& AttributeInitializer)
{
	SLATE_ADD_MEMBER_ATTRIBUTE_DEFINITION(AttributeInitializer, ColorAndOpacity, EInvalidateWidgetReason::Paint);
}

SAdvImage::SAdvImage()
	: ColorAndOpacity(*this, FLinearColor::White)
	, bFlipForRightToLeftFlowDirection(false)
{
}

void SAdvImage::Construct(const FArguments& InArgs)
{
	Image                            = FInvalidatableBrushAttribute(InArgs._Image);
	bFlipForRightToLeftFlowDirection = InArgs._FlipForRightToLeftFlowDirection;

	SetColorAndOpacity(InArgs._ColorAndOpacity);
	SetOnMouseButtonDown(InArgs._OnMouseButtonDown);

	// Call the parent constructor with our slots
	Super::FArguments ParentArgs;
	ParentArgs._Slots = MoveTemp(const_cast<FArguments&>(InArgs)._Slots);
	Super::Construct(ParentArgs);
}

void SAdvImage::SetColorAndOpacity(const TAttribute<FSlateColor>& InColorAndOpacity)
{
	ColorAndOpacity.Assign(*this, InColorAndOpacity);
}

void SAdvImage::SetColorAndOpacity(FLinearColor InColorAndOpacity)
{
	SetColorAndOpacity(TAttribute<FSlateColor>(InColorAndOpacity));
}

void SAdvImage::SetImage(TAttribute<const FSlateBrush*> InImage)
{
	Image.SetImage(*this, InImage);
}

int32 SAdvImage::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	const bool         bIsEnabled = ShouldBeEnabled(bParentEnabled);
	const FSlateBrush* ImageBrush = Image.GetImage().Get();

	if ((ImageBrush != nullptr) && (ImageBrush->DrawAs != ESlateBrushDrawType::NoDrawType))
	{
		const ESlateDrawEffect DrawEffects = bIsEnabled ? ESlateDrawEffect::None : ESlateDrawEffect::DisabledEffect;

		const FLinearColor FinalColorAndOpacity(InWidgetStyle.GetColorAndOpacityTint() * ColorAndOpacity.Get().GetColor(InWidgetStyle) * ImageBrush->GetTint(InWidgetStyle));

		if (bFlipForRightToLeftFlowDirection && GSlateFlowDirection == EFlowDirection::RightToLeft)
		{
			const FGeometry FlippedGeometry = AllottedGeometry.MakeChild(FSlateRenderTransform(FScale2D(-1, 1)));
			FSlateDrawElement::MakeBox(OutDrawElements, LayerId, FlippedGeometry.ToPaintGeometry(), ImageBrush, DrawEffects, FinalColorAndOpacity);
		}
		else
		{
			FSlateDrawElement::MakeBox(OutDrawElements, LayerId, AllottedGeometry.ToPaintGeometry(), ImageBrush, DrawEffects, FinalColorAndOpacity);
		}
	}

	return PaintChildren(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId + 1, InWidgetStyle, bIsEnabled);
}

FVector2D SAdvImage::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
	// Return the size of children or the size of the image, whichever is bigger
	const FVector2D& ContentSize = Super::ComputeDesiredSize(LayoutScaleMultiplier);
	const FSlateBrush* ImageBrush = Image.Get();
	if (ImageBrush != nullptr)
	{
		const FVector2D& ImageSize = ImageBrush->ImageSize;

		const FVector2D MaxSize(
			FMath::Max(ContentSize.X, ImageSize.X),
			FMath::Max(ContentSize.Y, ImageSize.Y)
		);
		return MaxSize;
	}
	return ContentSize;
}
