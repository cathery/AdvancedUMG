// Copyright (c) 2022 cathery

#pragma once

#include "CoreMinimal.h"

// The base class for AdvHorizontalBox and AdvVerticalBox. Contains all the logic of displaying objects in order.
class SAdvPanelBox : public SPanel
{
	SLATE_DECLARE_WIDGET_API(SAdvPanelBox, SPanel, ADVANCEDUMG_API)

public:
	/** Stores the per-child info for this panel type */
	struct FSlot : public TBasicLayoutWidgetSlot<FSlot>
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
			: TBasicLayoutWidgetSlot<FSlot>(HAlign_Fill, VAlign_Fill)
			, SizeParam(FStretch(1))
		{
		}

		SLATE_SLOT_BEGIN_ARGS(FSlot, TBasicLayoutWidgetSlot<FSlot>)
			SLATE_ARGUMENT(TOptional<FSizeParam>, SizeParam)
		SLATE_SLOT_END_ARGS()

		void Construct(const FChildren& SlotOwner, FSlotArguments&& InArgs);
		static void RegisterAttributes(FSlateWidgetSlotAttributeInitializer& AttributeInitializer);
	};

	SLATE_BEGIN_ARGS(SAdvPanelBox)
			: _Spacing(0.0f)
			, _Orientation(Orient_Horizontal)
		{
			_Visibility = EVisibility::SelfHitTestInvisible;
		}

		SLATE_SLOT_ARGUMENT(FSlot, Slots)

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

	static FSlot::FSlotArguments Slot()
	{
		return FSlot::FSlotArguments(MakeUnique<FSlot>());
	}

	using FScopedWidgetSlotArguments = TPanelChildren<FSlot>::FScopedWidgetSlotArguments;
	/**
	 * Adds a content slot.
	 *
	 * @return The added slot.
	 */
	FScopedWidgetSlotArguments AddSlot()
	{
		return FScopedWidgetSlotArguments(MakeUnique<FSlot>(), Children, INDEX_NONE);
	}

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

	void SetOrientation(const TAttribute<EOrientation>& InOrientation);

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
	TSlateAttribute<float> Spacing;

	/** The Box Panel's orientation; determined at construct time. */
	TSlateAttribute<EOrientation> Orientation;
};
