// Copyright (c) 2022 cathery

#include "Slate/SAdvPanelPadding.h"

#include "Layout/LayoutUtils.h"

void SAdvPanelPadding::Construct(const FArguments& InArgs)
{
	Padding = InArgs._Padding;

	// Call the parent constructor with our slots
	SAdvPanel::FArguments ParentArgs;
	ParentArgs.Slots = InArgs.Slots;
	SAdvPanel::Construct(ParentArgs);
}

void SAdvPanelPadding::SetPadding(const TAttribute<FMargin>& InPadding)
{
	SetAttribute(Padding, InPadding, EInvalidateWidgetReason::Layout);
}

void SAdvPanelPadding::OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const
{
	const EFlowDirection InFlowDirection = GSlateFlowDirection;

	for (int32 ChildIndex = 0; ChildIndex < Children.Num(); ++ChildIndex)
	{
		const FSlot&               CurChild  = Children[ChildIndex];
		const TSharedRef<SWidget>& CurWidget = CurChild.GetWidget();

		const EVisibility ChildVisibility = CurWidget->GetVisibility();
		if (ArrangedChildren.Accepts(ChildVisibility))
		{
			const FMargin SlotPadding(LayoutPaddingWithFlow(InFlowDirection, Padding.Get() + CurChild.SlotPadding.Get()));
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

FVector2D SAdvPanelPadding::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
	return SAdvPanel::ComputeDesiredSize(LayoutScaleMultiplier) + Padding.Get().GetDesiredSize();
}
