// Copyright (c) 2022 cathery

#include "Slate/SAdvPanelBox.h"

#include "Layout/LayoutUtils.h"

SAdvPanelBox::SAdvPanelBox()
	: Children(this)
{
	SetCanTick(false);
	bCanSupportFocus = false;
}

void SAdvPanelBox::Construct(const FArguments& InArgs)
{
	Spacing              = InArgs._Spacing;
	Orientation          = InArgs._Orientation;
	const int32 NumSlots = InArgs.Slots.Num();
	for (int32 SlotIndex = 0; SlotIndex < NumSlots; ++SlotIndex)
	{
		Children.Add(InArgs.Slots[SlotIndex]);
	}
}

SAdvPanelBox::FSlot& SAdvPanelBox::AddSlot()
{
	Invalidate(EInvalidateWidget::Layout);

	FSlot& NewSlot = *(new FSlot());
	this->Children.Add(&NewSlot);
	return NewSlot;
}

int32 SAdvPanelBox::RemoveSlot(const TSharedRef<SWidget>& SlotWidget)
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

void SAdvPanelBox::ClearChildren()
{
	if (Children.Num())
	{
		Invalidate(EInvalidateWidget::Layout);
		Children.Empty();
	}
}

void SAdvPanelBox::SetSpacing(const TAttribute<float>& InSpacing)
{
	SetAttribute(Spacing, InSpacing, EInvalidateWidgetReason::Layout);
}

void SAdvPanelBox::OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const
{
	// Allotted space will be given to fixed-size children first.
	// Remaining space will be proportionately divided between stretch children (SizeRule_Stretch)
	// based on their stretch coefficient
	const EFlowDirection InLayoutFlow   = GSlateFlowDirection;
	const EOrientation   CurOrientation = Orientation.Get();

	if (Children.Num() > 0)
	{
		bool bAtLeastOneChildVisible = false;

		float StretchCoefficientTotal = 0;

		float FixedTotal = 0;

		// Compute the sum of stretch coefficients (SizeRule_Stretch) and space required by fixed-size widgets
		// (SizeRule_Auto).
		for (int32 ChildIndex = 0; ChildIndex < Children.Num(); ++ChildIndex)
		{
			const FSlot& CurChild = Children[ChildIndex];

			if (CurChild.GetWidget()->GetVisibility() != EVisibility::Collapsed)
			{
				// All widgets contribute their margin to the fixed space requirement
				if (Orientation.Get() == Orient_Vertical)
				{
					FixedTotal += CurChild.SlotPadding.Get().GetTotalSpaceAlong<Orient_Vertical>();
				}
				else
				{
					FixedTotal += CurChild.SlotPadding.Get().GetTotalSpaceAlong<Orient_Horizontal>();
				}

				if (CurChild.SizeParam.SizeRule == FSizeParam::SizeRule_Stretch)
				{
					// for stretch children we save sum up the stretch coefficients
					StretchCoefficientTotal += CurChild.SizeParam.Value.Get();
				}
				else
				{
					FVector2D ChildDesiredSize = CurChild.GetWidget()->GetDesiredSize();

					// Auto-sized children contribute their desired size to the fixed space requirement
					const float ChildSize = (CurOrientation == Orient_Vertical)
					? ChildDesiredSize.Y
					: ChildDesiredSize.X;

					FixedTotal += ChildSize;
				}
			}
			bAtLeastOneChildVisible ? FixedTotal += Spacing.Get() : bAtLeastOneChildVisible = true;
		}

		if (!bAtLeastOneChildVisible)
		{
			return;
		}

		// The space available for SizeRule_Stretch widgets is any space that wasn't taken up by fixed-sized widgets.
		const float NonFixedSpace = FMath::Max(0.0f, (CurOrientation == Orient_Vertical)
			? AllottedGeometry.GetLocalSize().Y - FixedTotal
			: AllottedGeometry.GetLocalSize().X - FixedTotal);

		float PositionSoFar = 0;

		// Now that we have the total fixed-space requirement and the total stretch coefficients we can
		// arrange widgets top-to-bottom or left-to-right (depending on the orientation).
		for (TPanelChildrenConstIterator<FSlot> It(Children, CurOrientation, InLayoutFlow); It; ++It)
		{
			const FSlot&      CurChild        = *It;
			const EVisibility ChildVisibility = CurChild.GetWidget()->GetVisibility();

			// Figure out the area allocated to the child in the direction of BoxPanel
			// The area allocated to the slot is ChildSize + the associated margin.
			float ChildSize = 0;
			if (ChildVisibility != EVisibility::Collapsed)
			{
				// The size of the widget depends on its size type
				if (CurChild.SizeParam.SizeRule == FSizeParam::SizeRule_Stretch)
				{
					if (StretchCoefficientTotal > 0)
					{
						// Stretch widgets get a fraction of the space remaining after all the fixed-space requirements are met
						ChildSize = NonFixedSpace * CurChild.SizeParam.Value.Get() / StretchCoefficientTotal;
					}
				}
				else
				{
					const FVector2D ChildDesiredSize = CurChild.GetWidget()->GetDesiredSize();

					// Auto-sized widgets get their desired-size value
					ChildSize = (CurOrientation == Orient_Vertical)
					? ChildDesiredSize.Y
					: ChildDesiredSize.X;
				}
			}

			const FMargin SlotPadding(LayoutPaddingWithFlow(InLayoutFlow, CurChild.SlotPadding.Get()));

			FVector2D SlotSize = (Orientation.Get() == Orient_Vertical)
				? FVector2D(AllottedGeometry.GetLocalSize().X, ChildSize + SlotPadding.GetTotalSpaceAlong<Orient_Vertical>())
				: FVector2D(ChildSize + SlotPadding.GetTotalSpaceAlong<Orient_Horizontal>(), AllottedGeometry.GetLocalSize().Y);

			// Figure out the size and local position of the child within the slot
			AlignmentArrangeResult XAlignmentResult = AlignChild<Orient_Horizontal>(InLayoutFlow, SlotSize.X, CurChild, SlotPadding);
			AlignmentArrangeResult YAlignmentResult = AlignChild<Orient_Vertical>(SlotSize.Y, CurChild, SlotPadding);

			const FVector2D LocalPosition = (CurOrientation == Orient_Vertical)
			? FVector2D(XAlignmentResult.Offset, PositionSoFar + YAlignmentResult.Offset)
			: FVector2D(PositionSoFar + XAlignmentResult.Offset, YAlignmentResult.Offset);

			const FVector2D LocalSize = FVector2D(XAlignmentResult.Size, YAlignmentResult.Size);

			// Add the information about this child to the output list (ArrangedChildren)
			ArrangedChildren.AddWidget(ChildVisibility, AllottedGeometry.MakeChild(
				// The child widget being arranged
				CurChild.GetWidget(),
				// Child's local position (i.e. position within parent)
				LocalPosition,
				// Child's size
				LocalSize
			));

			if (ChildVisibility != EVisibility::Collapsed)
			{
				// Offset the next child by the size of the current child and any post-child (bottom/right) margin
				PositionSoFar += (CurOrientation == Orient_Vertical) ? SlotSize.Y : SlotSize.X;
				PositionSoFar += Spacing.Get();
			}
		}
	}
}

int32 SAdvPanelBox::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	// The panel has no visualization of its own; it just visualizes its children.
	FArrangedChildren ArrangedChildren(EVisibility::Visible);
	ArrangeChildren(AllottedGeometry, ArrangedChildren);

	// We want to draw the next child on top of all others, so we'll keep track of the max layer achieved
	int32 MaxLayerId = LayerId;

	const FPaintArgs NewArgs = Args.WithNewParent(this);

	const bool bShouldBeEnabled = ShouldBeEnabled(bParentEnabled);

	bool bAtLeastOneChildVisible = false;

	for (int32 ChildIndex = 0; ChildIndex < ArrangedChildren.Num(); ++ChildIndex)
	{
		const FArrangedWidget& CurWidget = ArrangedChildren[ChildIndex];

		if (!IsChildWidgetCulled(MyCullingRect, CurWidget))
		{
			// Only increment the max layer after the first visible child
			bAtLeastOneChildVisible ? ++MaxLayerId : bAtLeastOneChildVisible = true;

			const int32 CurWidgetsMaxLayerId = CurWidget.Widget->Paint(NewArgs, CurWidget.Geometry, MyCullingRect, OutDrawElements, MaxLayerId, InWidgetStyle, bShouldBeEnabled);

			MaxLayerId = FMath::Max(MaxLayerId, CurWidgetsMaxLayerId);
		}
	}

	return MaxLayerId;
}

FVector2D SAdvPanelBox::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
	// The desired size of this panel is the total size desired by its children plus any margins specified in this panel.
	// The layout along the panel's axis is describe dy the SizeParam, while the perpendicular layout is described by the
	// alignment property.
	FVector2D MyDesiredSize(0, 0);

	bool bAtLeastOneChildVisible = false;

	for (int32 ChildIndex = 0; ChildIndex < Children.Num(); ++ChildIndex)
	{
		const FSlot& CurChild = Children[ChildIndex];

		if (CurChild.GetWidget()->GetVisibility() != EVisibility::Collapsed)
		{
			const FVector2D& CurChildDesiredSize = CurChild.GetWidget()->GetDesiredSize();

			if (Orientation.Get() == Orient_Vertical)
			{
				// For a vertical panel, we want to find the maximum desired width (including margin).
				// That will be the desired width of the whole panel.
				MyDesiredSize.X = FMath::Max(MyDesiredSize.X, CurChildDesiredSize.X + CurChild.SlotPadding.Get().GetTotalSpaceAlong<Orient_Horizontal>());

				MyDesiredSize.Y += CurChildDesiredSize.Y + CurChild.SlotPadding.Get().GetTotalSpaceAlong<Orient_Vertical>();
				bAtLeastOneChildVisible ? MyDesiredSize.Y += Spacing.Get() : bAtLeastOneChildVisible = true;
			}
			else
			{
				// A horizontal panel is just a sideways vertical panel: the axes are swapped.
				MyDesiredSize.Y = FMath::Max(MyDesiredSize.Y, CurChildDesiredSize.Y + CurChild.SlotPadding.Get().GetTotalSpaceAlong<Orient_Vertical>());

				MyDesiredSize.X += CurChildDesiredSize.X + CurChild.SlotPadding.Get().GetTotalSpaceAlong<Orient_Horizontal>();
				bAtLeastOneChildVisible ? MyDesiredSize.X += Spacing.Get() : bAtLeastOneChildVisible = true;
			}
		}
	}

	return MyDesiredSize;
}

FChildren* SAdvPanelBox::GetChildren()
{
	return &Children;
}
