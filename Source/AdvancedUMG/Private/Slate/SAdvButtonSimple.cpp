// Copyright (c) 2022 cathery

#include "Slate/SAdvButtonSimple.h"

#include "Layout/LayoutUtils.h"

SLATE_IMPLEMENT_WIDGET(SAdvButtonSimple)
void SAdvButtonSimple::PrivateRegisterAttributes(FSlateAttributeInitializer& AttributeInitializer)
{
}

SAdvButtonSimple::SAdvButtonSimple()
	: Style(nullptr)
{
}

/**
 * Construct this widget
 *
 * @param	InArgs	The declaration data for this widget
 */
void SAdvButtonSimple::Construct(const FArguments& InArgs)
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
}

void SAdvButtonSimple::SetStyle(const FAdvButtonSimpleStyle* InStyle)
{
	Style = InStyle;
	Invalidate(EInvalidateWidgetReason::Layout);
}

void SAdvButtonSimple::OnClick()
{
	Super::OnClick();
	PlaySound(Style->ClickedSound);
}

void SAdvButtonSimple::OnPress()
{
	Super::OnPress();
	PlaySound(Style->PressedSound);
}

void SAdvButtonSimple::OnHover()
{
	Super::OnHover();
	PlaySound(Style->HoveredSound);
}

void SAdvButtonSimple::OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const
{
	const EFlowDirection InFlowDirection = GSlateFlowDirection;

	for (int32 ChildIndex = 0; ChildIndex < Children.Num(); ++ChildIndex)
	{
		const FSlot&               CurChild  = Children[ChildIndex];
		const TSharedRef<SWidget>& CurWidget = CurChild.GetWidget();

		const EVisibility ChildVisibility = CurWidget->GetVisibility();
		if (ArrangedChildren.Accepts(ChildVisibility))
		{
			const FMargin SlotPadding(LayoutPaddingWithFlow(InFlowDirection, Style->ContentPadding + CurChild.GetPadding()));
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

int32 SAdvButtonSimple::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	const bool bEnabled                = ShouldBeEnabled(bParentEnabled);
	const ESlateDrawEffect DrawEffects = Style->bShowDisabledEffect && !bEnabled ? ESlateDrawEffect::DisabledEffect : ESlateDrawEffect::None;

	int32 MaxLayerId = LayerId;

	const FGeometry& PaddedGeometry = bIsPressed ? AllottedGeometry.MakeChild(AllottedGeometry.GetLocalSize(), FSlateLayoutTransform(FVector2D(0.0f, Style->DownNudge))) : AllottedGeometry;

	if (Style && Style->Image.DrawAs != ESlateBrushDrawType::NoDrawType)
	{
		FSlateDrawElement::MakeBox(
			OutDrawElements,
			MaxLayerId++,
			PaddedGeometry.ToPaintGeometry(),
			&Style->Image,
			DrawEffects,
			Style->Image.GetTint(InWidgetStyle) * InWidgetStyle.GetColorAndOpacityTint()
		);
	}

	return PaintChildren(Args, PaddedGeometry, MyCullingRect, OutDrawElements, MaxLayerId++, InWidgetStyle, true);
}

FVector2D SAdvButtonSimple::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
	// Return the size of children or the size of the image, whichever is bigger
	const FVector2D& ContentSize = SAdvPanel::ComputeDesiredSize(LayoutScaleMultiplier);
	const FVector2D& ButtonSize  = Style->Image.ImageSize;

	const FVector2D MaxSize(
		FMath::Max(ContentSize.X, ButtonSize.X),
		FMath::Max(ContentSize.Y, ButtonSize.Y)
	);

	return MaxSize + Style->ContentPadding.GetDesiredSize();
}
