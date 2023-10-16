// Copyright (c) 2022 cathery

#include "Slate/SAdvPanelPadding.h"

#include "Layout/LayoutUtils.h"

SLATE_IMPLEMENT_WIDGET(SAdvPanelPadding)

void SAdvPanelPadding::PrivateRegisterAttributes(FSlateAttributeInitializer& AttributeInitializer)
{
	SLATE_ADD_MEMBER_ATTRIBUTE_DEFINITION(AttributeInitializer, Padding, EInvalidateWidgetReason::Layout);
}

SAdvPanelPadding::SAdvPanelPadding()
	: Padding(*this, FMargin())
{
}

void SAdvPanelPadding::Construct(const FArguments& InArgs)
{
	SetPadding(InArgs._Padding);

	// Call the parent constructor with our slots
	Super::FArguments ParentArgs;
	ParentArgs._Slots = MoveTemp(const_cast<FArguments&>(InArgs)._Slots);
	Super::Construct(ParentArgs);
}

void SAdvPanelPadding::SetPadding(const TAttribute<FMargin>& InPadding)
{
	Padding.Assign(*this, InPadding);
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
			const FMargin SlotPadding(LayoutPaddingWithFlow(InFlowDirection, Padding.Get() + CurChild.GetPadding()));
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
	return Super::ComputeDesiredSize(LayoutScaleMultiplier) + Padding.Get().GetDesiredSize();
}
