// Copyright (c) 2022 cathery

#include "Slate/SAdvPanel.h"

#include "Layout/LayoutUtils.h"

SAdvPanel::SAdvPanel()
	: Children(this)
{
	SetCanTick(false);
	bCanSupportFocus = false;
}

void SAdvPanel::Construct(const FArguments& InArgs)
{
	const int32 NumSlots = InArgs.Slots.Num();
	for (int32 SlotIndex = 0; SlotIndex < NumSlots; ++SlotIndex)
	{
		Children.Add(InArgs.Slots[SlotIndex]);
	}
}

SAdvPanel::FSlot& SAdvPanel::AddSlot()
{
	Invalidate(EInvalidateWidget::Layout);

	FSlot& NewSlot = *(new FSlot());
	this->Children.Add(&NewSlot);
	return NewSlot;
}

int32 SAdvPanel::RemoveSlot(const TSharedRef<SWidget>& SlotWidget)
{
	Invalidate(EInvalidateWidget::Layout);

	for (int32 SlotIdx = 0; SlotIdx < Children.Num(); ++SlotIdx)
	{
		if (SlotWidget == Children[SlotIdx].GetWidget())
		{
			Children.RemoveAt(SlotIdx);
			return SlotIdx;
		}
	}

	return -1;
}

void SAdvPanel::ClearChildren()
{
	if (Children.Num())
	{
		Invalidate(EInvalidateWidget::Layout);
		Children.Empty();
	}
}

int32 SAdvPanel::GetNumChildren() const
{
	return Children.Num();
}

FChildren* SAdvPanel::GetChildren()
{
	return &Children;
}


void SAdvPanel::OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const
{
	for (int32 ChildIndex = 0; ChildIndex < Children.Num(); ++ChildIndex)
	{
		const FSlot&               CurChild        = Children[ChildIndex];
		const TSharedRef<SWidget>& CurWidget       = CurChild.GetWidget();
		const EFlowDirection       InFlowDirection = GSlateFlowDirection;

		const EVisibility ChildVisibility = CurWidget->GetVisibility();
		if (ArrangedChildren.Accepts(ChildVisibility))
		{
			const FMargin SlotPadding(LayoutPaddingWithFlow(InFlowDirection, CurChild.SlotPadding.Get()));
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

int32 SAdvPanel::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	return PaintChildren(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, ShouldBeEnabled(bParentEnabled));
}

FVector2D SAdvPanel::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
	FVector2D MaxSize(0, 0);

	// Go through all our children and find their biggest size
	for (int32 ChildIndex = 0; ChildIndex < Children.Num(); ++ChildIndex)
	{
		const FSlot&               CurChild       = Children[ChildIndex];
		const TSharedRef<SWidget>& CurWidget      = CurChild.GetWidget();
		const EVisibility          ChildVisibilty = CurWidget->GetVisibility();

		// As long as the widgets are not collapsed, they should contribute to the desired size.
		if (ChildVisibilty != EVisibility::Collapsed)
		{
			const FVector2D ChildSize = CurWidget->GetDesiredSize() + CurChild.SlotPadding.Get().GetDesiredSize();

			MaxSize.X = FMath::Max(MaxSize.X, ChildSize.X);
			MaxSize.Y = FMath::Max(MaxSize.Y, ChildSize.Y);
		}
	}

	return MaxSize;
}

int32 SAdvPanel::PaintChildren(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bShouldChildrenBeEnabled) const
{
	// The panel has no visualization of its own; it just visualizes its children.
	FArrangedChildren ArrangedChildren(EVisibility::Visible);
	ArrangeChildren(AllottedGeometry, ArrangedChildren);

	// We want to draw the next child on top of all others, so we'll keep track of the max layer achieved
	int32 MaxLayerId = LayerId;

	const FPaintArgs NewArgs = Args.WithNewParent(this);

	bool bAtLeastOneChildVisible = false;

	for (int32 ChildIndex = 0; ChildIndex < ArrangedChildren.Num(); ++ChildIndex)
	{
		const FArrangedWidget& CurWidget = ArrangedChildren[ChildIndex];

		if (!IsChildWidgetCulled(MyCullingRect, CurWidget))
		{
			// Only increment the max layer after the first visible child
			bAtLeastOneChildVisible ? ++MaxLayerId : bAtLeastOneChildVisible = true;

			const int32 CurWidgetsMaxLayerId = CurWidget.Widget->Paint(NewArgs, CurWidget.Geometry, MyCullingRect, OutDrawElements, MaxLayerId, InWidgetStyle, bShouldChildrenBeEnabled);

			MaxLayerId = FMath::Max(MaxLayerId, CurWidgetsMaxLayerId);
		}
	}

	return MaxLayerId;
}

void SAdvPanel::PlaySound(const FSlateSound& InSound)
{
	FSlateApplication::Get().PlaySound(InSound);
}

FReply SAdvPanel::OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	AdvMouseWheelEvent.ExecuteIfBound(MyGeometry, MouseEvent);

	return SPanel::OnMouseWheel(MyGeometry, MouseEvent);
}

