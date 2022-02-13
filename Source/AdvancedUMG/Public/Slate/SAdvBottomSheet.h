// Copyright (c) 2022 cathery

#pragma once

#include "CoreMinimal.h"

#include "Slate/SAdvImage.h"
#include "Slate/SAdvPanel.h"

class SAdvColorRect;
class SAdvBottomSheetScrollPanel;

/**
 * Basic container widget that lets you define padding for child elements.
 */
class ADVANCEDUMG_API SAdvBottomSheet : public SAdvPanel
{
public:
	SLATE_BEGIN_ARGS(SAdvBottomSheet)
			: _Background(FCoreStyle::Get().GetDefaultBrush())
			, _DragThreshold(25.0f)
			, _MoveThreshold(50.0f)
			, _SheetInterpSpeed(5.0f)
			, _ScrollInterpSpeed(5.0f)
			, _SheetSpeedMultiplier(1.0f)
			, _ScrollSpeedMultiplier(1.0f)
			, _SheetTargetAreas({0.1f, 0.25f, 0.75f})
		{
		}

		/** Background image */
		SLATE_ARGUMENT(const FSlateBrush*, Background)

		/** How much the mouse/finger should move before we register it as a drag gesture */
		SLATE_ARGUMENT(float, DragThreshold)

		/** How much the mouse/finger should move before the sheet recognizes it as an extend/retract gesture */
		SLATE_ARGUMENT(float, MoveThreshold)

		/** How fast the sheet animation should catch up with the movement */
		SLATE_ARGUMENT(float, SheetInterpSpeed)

		/** How fast the scroll animation should catch up with the movement */
		SLATE_ARGUMENT(float, ScrollInterpSpeed)

		/** How much should the sheet move from scrolling */
		SLATE_ARGUMENT(float, SheetSpeedMultiplier)

		/** How much should the content move from scrolling */
		SLATE_ARGUMENT(float, ScrollSpeedMultiplier)

		/** The list of target areas the sheet should snap to */
		SLATE_ARGUMENT(TArray<float>, SheetTargetAreas)

		SLATE_SUPPORTS_SLOT(SAdvPanel::FSlot)
	SLATE_END_ARGS()

public:
	SAdvBottomSheet();

	/**
	 * Construct this widget
	 *
	 * @param	InArgs	The declaration data for this widget
	 */
	void Construct(const FArguments& InArgs);

	/**
	 * Adds a content slot.
	 *
	 * @param bIsPersistent Should the slot always be on top of the sheet?
	 * @return The added slot.
	 */
	FSlot& AddSheetSlot(bool bIsPersistent);

	/**
	 * Removes a particular content slot.
	 *
	 * @param SlotWidget The widget in the slot to remove.
	 * @param bIsPersistent Is the slot persistent?
	 */
	int32 RemoveSheetSlot(const TSharedRef<SWidget>& SlotWidget, bool bIsPersistent);

	/** See the Background attribute */
	void SetBackground(const FSlateBrush& InBackground);

	/** See the DragThreshold attribute */
	void SetDragThreshold(float InDragThreshold);

	/** See the MoveThreshold attribute */
	void SetMoveThreshold(float InMoveThreshold);

	/** See the SheetInterpSpeed attribute */
	void SetSheetInterpSpeed(float InSheetInterpSpeed);

	/** See the ScrollInterpSpeed attribute */
	void SetScrollInterpSpeed(float InScrollInterpSpeed);

	/** See the SheetSpeedMultiplier attribute */
	void SetSheetSpeedMultiplier(float InSheetSpeedMultiplier);

	/** See the ScrollSpeedMultiplier attribute */
	void SetScrollSpeedMultiplier(float InScrollSpeedMultiplier);

	/** See the SheetTargetAreas attribute */
	void SetSheetTargetAreas(const TArray<float>& InSheetTargetAreas);

	/** Set the sheet offset */
	void SetSheetOffset(float InSheetOffset);

	/** Set the scroll (content) offset */
	void SetScrollOffset(float InScrollOffset);

public:
	void InitHeight(const TOptional<const FGeometry> MyGeometry = {});

	int32 FindTargetAreaToSnapTo(const FGeometry& MyGeometry, float Height, float DeltaHeight);

	float FindSheetMinimumHeight(const FGeometry& MyGeometry) const;
	float FindSheetMaximumHeight(const FGeometry& MyGeometry) const;
	float FindScrollPanelMinimumOffset(const FGeometry& MyGeometry) const;
	float FindScrollPanelMaximumOffset(const FGeometry& MyGeometry) const;
	float FindTotalMinimumOffset(const FGeometry& MyGeometry) const;
	float FindTotalMaximumOffset(const FGeometry& MyGeometry) const;

public:
	//~ Begin SWidget Interface
	virtual void   Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
	virtual FReply OnPreviewMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void   OnMouseCaptureLost(const FCaptureLostEvent& CaptureLostEvent) override;
	virtual bool   IsInteractable() const override;
	//~ End SWidget Interface

protected:
	//~ Begin SPanel Interface
	virtual FVector2D ComputeDesiredSize(float LayoutScaleMultiplier) const override;
	//~ End SPanel Interface

protected:
	/** How much the mouse/finger should move before we register it as a drag gesture */
	float DragThreshold;

	/** How much the mouse/finger should move before the sheet recognizes it as an extend/retract gesture */
	float MoveThreshold;

	/** How fast the sheet animation should catch up with the movement */
	float SheetInterpSpeed;

	/** How fast the scroll animation should catch up with the movement */
	float ScrollInterpSpeed;

	/** How much should the sheet move from scrolling */
	float SheetSpeedMultiplier;

	/** How much should the content move from scrolling */
	float ScrollSpeedMultiplier;

	/** The list of target areas the sheet should snap to */
	TArray<float> SheetTargetAreas;

protected:
	/** Separate widget to contain the scrollable sheet content */
	TSharedPtr<SAdvBottomSheetScrollPanel> SheetScrollPanel;

	/** Separate widget to contain all the sheet content */
	TSharedPtr<SAdvPanel> SheetPanel;

	/** The slot for SheetPanel, used to quickly access margin values */
	FSlot* SheetPanelSlot;

	/** Separate widget to block clicks outside of the sheet */
	TSharedPtr<SAdvColorRect> OffscreenClickBlocker;

	// Is the sheet currently being pressed?
	bool bIsPressed;

	// Is the sheet currently being dragged?
	bool bIsDragging;


	// The total amount of vertical mouse movement since the sheet was pressed
	float TotalMouseDelta;


	// The current sheet offset, animated each tick towards the TargetSheetOffset
	float CurrentSheetOffset;

	// The target sheet offset, aka the real sheet offset (without SheetDelta applied)
	float TargetSheetOffset;

	// How much the TargetSheetOffset should be modified, this is modified by scrolling movements and gets added to TargetSheetOffset when finished
	float SheetDelta;


	// The current scroll offset, animated each tick towards the TargetScrollOffset
	float CurrentScrollOffset;

	// The target scroll offset, aka the real scroll offset (without ScrollDelta applied)
	float TargetScrollOffset;

	// How much the TargetScrollOffset should be modified, this is modified by scrolling movements and gets added to TargetScrollOffset when finished
	float ScrollDelta;

	// The current area the sheet is snapped to
	int32 CurrentSheetTargetAreaIndex;
};

class ADVANCEDUMG_API SAdvBottomSheetScrollPanel : public SAdvImage
{
public:
	SAdvBottomSheetScrollPanel();

	void Construct(const FArguments& InArgs, const TArray<FSlot*>& InSlots);

	virtual void OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const override;

public:
	float PhysicalOffset;
};
