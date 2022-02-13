// Copyright (c) 2022 cathery

#pragma once

#include "CoreMinimal.h"

// The base class for AdvHorizontalBox and AdvVerticalBox. Contains all the logic of displaying objects in order.
class ADVANCEDUMG_API SAdvPanelBox : public SPanel
{
public:
	/** Stores the per-child info for this panel type */
	struct FSlot : public TSlotBase<FSlot>, public TSupportsContentAlignmentMixin<FSlot>, public TSupportsContentPaddingMixin<FSlot>
	{
		virtual ~FSlot() override
		{
		}

		/**
		* How much space this slot should occupy along panel's direction.
		*   When SizeRule is SizeRule_Auto, the widget's DesiredSize will be used as the space required.
		*   When SizeRule is SizeRule_Stretch, the available space will be distributed proportionately between
		*   peer Widgets depending on the Value property. Available space is space remaining after all the
		*   peers' SizeRule_Auto requirements have been satisfied.
		*/
		FSizeParam SizeParam;

		FSlot()
			: TSlotBase<FSlot>()
			, TSupportsContentAlignmentMixin<FSlot>(HAlign_Fill, VAlign_Fill)
			, SizeParam(FStretch(1))
		{
		}
	};

	SLATE_BEGIN_ARGS(SAdvPanelBox)
			: _Spacing(0.0f)
			, _Orientation(Orient_Horizontal)
		{
			_Visibility = EVisibility::SelfHitTestInvisible;
		}

		SLATE_SUPPORTS_SLOT(SAdvPanelBox::FSlot)

		SLATE_ATTRIBUTE(float, Spacing)
		SLATE_ATTRIBUTE(EOrientation, Orientation)
	SLATE_END_ARGS()

	/** Default constructor. */
	SAdvPanelBox();

	/**
	 * Construct this widget
	 *
	 * @param	InArgs	The declaration data for this widget
	 */
	void Construct(const FArguments& InArgs);

	static FSlot& Slot()
	{
		return *(new FSlot());
	}

	/**
	 * Adds a content slot.
	 *
	 * @return The added slot.
	 */
	FSlot& AddSlot();

	/**
	 * Removes a particular content slot.
	 *
	 * @param SlotWidget The widget in the slot to remove.
	 */
	int32 RemoveSlot(const TSharedRef<SWidget>& SlotWidget);

	/**
	 * Removes all slots from the panel.
	 */
	void ClearChildren();

	//~ Begin SPanel Interface
	virtual FChildren* GetChildren() override;
	//~ End SPanel Interface

	void SetSpacing(const TAttribute<float>& InSpacing);

protected:
	//~ Begin SPanel Interface
	virtual void      OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const override;
	virtual int32     OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	virtual FVector2D ComputeDesiredSize(float) const override;
	//~ End SPanel Interface

protected:
	/** This widget's children. */
	TPanelChildren<FSlot> Children;

	/** The spacing between the child elements. */
	TAttribute<float> Spacing;

	/** The Box Panel's orientation; determined at construct time. */
	TAttribute<EOrientation> Orientation;
};
