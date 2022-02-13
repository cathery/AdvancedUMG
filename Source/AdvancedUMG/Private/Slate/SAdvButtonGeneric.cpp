// Copyright (c) 2022 cathery

#include "Slate/SAdvButtonGeneric.h"

#include "Layout/LayoutUtils.h"

#include "Slate/AdvButtonGenericStyle.h"

SAdvButtonGeneric::SAdvButtonGeneric()
	: Style(nullptr)
{
}

/**
 * Construct this widget
 *
 * @param	InArgs	The declaration data for this widget
 */
void SAdvButtonGeneric::Construct(const FArguments& InArgs)
{
	bIsPressed = false;

	// Call the parent constructor with our slots
	SAdvButtonBase::FArguments ParentArgs;
	ParentArgs.Slots = InArgs.Slots;
	ParentArgs
		.ClickMethod(InArgs._ClickMethod)
		.TouchMethod(InArgs._TouchMethod)
		.PressMethod(InArgs._PressMethod)
		.IsFocusable(InArgs._IsFocusable)
		.OnClicked(InArgs._OnClicked)
		.OnPressed(InArgs._OnPressed)
		.OnReleased(InArgs._OnReleased)
		.OnHovered(InArgs._OnHovered)
		.OnUnhovered(InArgs._OnUnhovered);
	SAdvButtonBase::Construct(ParentArgs);

	Style = InArgs._Style;

	ColorAndOpacity = InArgs._ColorAndOpacity;
	BackgroundColor = InArgs._BackgroundColor;
}

FMargin SAdvButtonGeneric::GetCombinedPadding() const
{
	return IsPressed() ? Style->PressedPadding : Style->NormalPadding;
}

bool SAdvButtonGeneric::GetShowDisabledEffect() const
{
	return Style->Disabled.DrawAs == ESlateBrushDrawType::NoDrawType;
}

/** @return An image that represents this button's border */
const FSlateBrush* SAdvButtonGeneric::GetBorder() const
{
	if (!GetShowDisabledEffect() && !IsEnabled())
	{
		return &Style->Disabled;
	}
	if (IsPressed())
	{
		return &Style->Pressed;
	}
	if (IsHovered())
	{
		return &Style->Hovered;
	}
	return &Style->Normal;
}

void SAdvButtonGeneric::SetButtonStyle(const FAdvButtonGenericStyle* ButtonStyle)
{
	/* Get pointer to the button style */
	Style = ButtonStyle;
	Invalidate(EInvalidateWidget::Layout);
}

void SAdvButtonGeneric::SetColorAndOpacity(const TAttribute<FLinearColor>& InColorAndOpacity)
{
	SetAttribute(ColorAndOpacity, InColorAndOpacity, EInvalidateWidgetReason::Paint);
}

void SAdvButtonGeneric::SetBorderBackgroundColor(const TAttribute<FLinearColor>& InBackgroundColor)
{
	SetAttribute(BackgroundColor, InBackgroundColor, EInvalidateWidgetReason::Paint);
}

void SAdvButtonGeneric::OnClick()
{
	SAdvButtonBase::OnClick();
	PlaySound(Style->ClickedSound);
}

void SAdvButtonGeneric::OnPress()
{
	SAdvButtonBase::OnPress();
	PlaySound(Style->PressedSound);
}

void SAdvButtonGeneric::OnHover()
{
	SAdvButtonBase::OnHover();
	PlaySound(Style->HoveredSound);
}

void SAdvButtonGeneric::OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const
{
	const EFlowDirection InFlowDirection = GSlateFlowDirection;

	for (int32 ChildIndex = 0; ChildIndex < Children.Num(); ++ChildIndex)
	{
		const FSlot&               CurChild  = Children[ChildIndex];
		const TSharedRef<SWidget>& CurWidget = CurChild.GetWidget();

		const EVisibility ChildVisibility = CurWidget->GetVisibility();
		if (ArrangedChildren.Accepts(ChildVisibility))
		{
			const FMargin SlotPadding(LayoutPaddingWithFlow(InFlowDirection, GetCombinedPadding() + CurChild.SlotPadding.Get()));
			const AlignmentArrangeResult XResult = AlignChild<Orient_Horizontal>(InFlowDirection, AllottedGeometry.GetLocalSize().X, CurChild, SlotPadding);
			const AlignmentArrangeResult YResult = AlignChild<Orient_Vertical>(AllottedGeometry.GetLocalSize().Y, CurChild, SlotPadding);

			ArrangedChildren.AddWidget(ChildVisibility, AllottedGeometry.MakeChild(
				CurWidget,
				FVector2D(XResult.Offset, YResult.Offset),
				FVector2D(XResult.Size, YResult.Size)
			));
		}
	}
}

int32 SAdvButtonGeneric::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	const bool bEnabled            = ShouldBeEnabled(bParentEnabled);
	const bool bShowDisabledEffect = GetShowDisabledEffect();

	// Children should be enabled if the button is enabled, or if disabled effect shouldn't be shown 
	const bool bShouldChildrenBeEnabled = bEnabled || !bShowDisabledEffect;

	const FSlateBrush* BrushResource = !bShowDisabledEffect && !bEnabled ? &Style->Disabled : GetBorder();

	const ESlateDrawEffect DrawEffects = bShowDisabledEffect && !bEnabled ? ESlateDrawEffect::DisabledEffect : ESlateDrawEffect::None;

	if (BrushResource && BrushResource->DrawAs != ESlateBrushDrawType::NoDrawType)
	{
		FSlateDrawElement::MakeBox(
			OutDrawElements,
			LayerId,
			AllottedGeometry.ToPaintGeometry(),
			BrushResource,
			DrawEffects,
			BrushResource->GetTint(InWidgetStyle) * InWidgetStyle.GetColorAndOpacityTint() * BackgroundColor.Get()
		);
	}

	const FWidgetStyle ChildrenWidgetStyle = FWidgetStyle(InWidgetStyle)
	.BlendColorAndOpacityTint(ColorAndOpacity.Get());

	return PaintChildren(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId + 1, ChildrenWidgetStyle, bShouldChildrenBeEnabled);
}

FVector2D SAdvButtonGeneric::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
	// Return the size of children or the size of the image, whichever is bigger
	const FVector2D& ContentSize = SAdvPanel::ComputeDesiredSize(LayoutScaleMultiplier);
	const FVector2D& ButtonSize  = GetBorder()->ImageSize;

	const FVector2D MaxSize(
		FMath::Max(ContentSize.X, ButtonSize.X),
		FMath::Max(ContentSize.Y, ButtonSize.Y)
	);

	return MaxSize + GetCombinedPadding().GetDesiredSize();
}
