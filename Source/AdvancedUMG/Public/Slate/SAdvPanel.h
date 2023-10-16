// Copyright (c) 2022 cathery

#pragma once

#include "CoreMinimal.h"

// Super basic Slate widget container that can have multiple children.
class SAdvPanel : public SPanel
{
	SLATE_DECLARE_WIDGET_API(SAdvPanel, SPanel, ADVANCEDUMG_API)

public:
	/** Stores the per-child info for this panel type */
	struct FSlot : public TBasicLayoutWidgetSlot<FSlot>
	{
		FSlot()
			: TBasicLayoutWidgetSlot<FSlot>(HAlign_Fill, VAlign_Fill)
		{
		}

		SLATE_SLOT_BEGIN_ARGS(FSlot, TBasicLayoutWidgetSlot<FSlot>)
		SLATE_SLOT_END_ARGS()

		void Construct(const FChildren& SlotOwner, FSlotArguments&& InArgs);
		static void RegisterAttributes(FSlateWidgetSlotAttributeInitializer& AttributeInitializer);
	};

	SLATE_BEGIN_ARGS(SAdvPanel)
		{
			_Visibility = EVisibility::SelfHitTestInvisible;
		}

		SLATE_SLOT_ARGUMENT(FSlot, Slots)

	SLATE_END_ARGS()

	/** Default constructor. */
	SAdvPanel();

	FNoReplyPointerEventHandler AdvMouseWheelEvent;

	/**
	 * Constructs this widget
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
	virtual int32 RemoveSlot(const TSharedRef<SWidget>& SlotWidget);

	/**
	 * Removes all slots from the panel.
	 */
	void ClearChildren();

	/** Returns the number of child widgets */
	int32 GetNumChildren() const;

	//~ Begin SPanel Interface
	virtual FChildren* GetChildren() override;
	//~ End SPanel Interface

protected:
	//~ Begin SPanel Interface
	virtual void      OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const override;
	virtual int32     OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	virtual FVector2D ComputeDesiredSize(float) const override;
	//~ End SPanel Interface

	// Helper function for inherited classes to paint our children
	int32 PaintChildren(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bShouldChildrenBeEnabled) const;

	static void PlaySound(const FSlateSound& InSound);

	virtual FReply OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

protected:
	/** This widget's children. */
	TPanelChildren<FSlot> Children;
};
