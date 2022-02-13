// Copyright (c) 2022 cathery

#pragma once

#include "CoreMinimal.h"

// Super basic Slate widget container that can have multiple children.
class ADVANCEDUMG_API SAdvPanel : public SPanel
{
public:
	/** Stores the per-child info for this panel type */
	struct FSlot : public TSlotBase<FSlot>, public TSupportsContentAlignmentMixin<FSlot>, public TSupportsContentPaddingMixin<FSlot>
	{
		FSlot()
			: TSlotBase<FSlot>()
			, TSupportsContentAlignmentMixin<FSlot>(HAlign_Fill, VAlign_Fill)
		{
		}
	};

	SLATE_BEGIN_ARGS(SAdvPanel)
		{
			_Visibility = EVisibility::SelfHitTestInvisible;
		}

		SLATE_SUPPORTS_SLOT(SAdvPanel::FSlot)
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

	static FSlot& Slot()
	{
		return *(new FSlot());
	}

	/**
	 * Adds a content slot.
	 *
	 * @return The added slot.
	 */
	virtual FSlot& AddSlot();

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
