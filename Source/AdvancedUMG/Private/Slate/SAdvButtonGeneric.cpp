// Copyright (c) 2022 cathery

#include "Slate/SAdvButtonGeneric.h"

#include "Layout/LayoutUtils.h"

#include "Slate/AdvButtonGenericStyle.h"

SLATE_IMPLEMENT_WIDGET(SAdvButtonGeneric)
void SAdvButtonGeneric::PrivateRegisterAttributes(FSlateAttributeInitializer& AttributeInitializer)
{
	SLATE_ADD_MEMBER_ATTRIBUTE_DEFINITION(AttributeInitializer, ColorAndOpacity, EInvalidateWidgetReason::Paint);
	SLATE_ADD_MEMBER_ATTRIBUTE_DEFINITION(AttributeInitializer, BackgroundColor, EInvalidateWidgetReason::Paint);
}

SAdvButtonGeneric::SAdvButtonGeneric()
	: Style(nullptr)
	, ColorAndOpacity(*this, FLinearColor::White)
	, BackgroundColor(*this, FLinearColor::White)
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
	Super::FArguments ParentArgs;
	ParentArgs._Slots = MoveTemp(const_cast<FArguments&>(InArgs)._Slots);
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
	Super::Construct(ParentArgs);

	Style = InArgs._Style;

	SetColorAndOpacity(InArgs._ColorAndOpacity);
	SetBorderBackgroundColor(InArgs._BackgroundColor);
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
	ColorAndOpacity.Assign(*this, InColorAndOpacity);
}

void SAdvButtonGeneric::SetBorderBackgroundColor(const TAttribute<FLinearColor>& InBackgroundColor)
{
	BackgroundColor.Assign(*this, InBackgroundColor);
}

void SAdvButtonGeneric::OnClick()
{
	Super::OnClick();
	PlaySound(Style->ClickedSound);
}

void SAdvButtonGeneric::OnPress()
{
	Super::OnPress();
	PlaySound(Style->PressedSound);
}

void SAdvButtonGeneric::OnHover()
{
	Super::OnHover();
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
			const FMargin SlotPadding(LayoutPaddingWithFlow(InFlowDirection, GetCombinedPadding() + CurChild.GetPadding()));
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
