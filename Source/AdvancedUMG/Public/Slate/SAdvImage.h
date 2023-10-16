// Copyright (c) 2022 cathery

#pragma once

#include "CoreMinimal.h"

#include "Slate/SAdvPanel.h"

// Image widget that lets you place children inside.
class SAdvImage : public SAdvPanel
{
	SLATE_DECLARE_WIDGET_API(SAdvImage, SAdvPanel, ADVANCEDUMG_API)

public:
	SLATE_BEGIN_ARGS(SAdvImage)
			: _Image(FCoreStyle::Get().GetDefaultBrush())
			, _ColorAndOpacity(FLinearColor::White)
			, _FlipForRightToLeftFlowDirection(false)
		{
		}

		/** Image resource */
		SLATE_ATTRIBUTE(const FSlateBrush*, Image)

		/** Color and opacity */
		SLATE_ATTRIBUTE(FSlateColor, ColorAndOpacity)

		/** Flips the image if the localization's flow direction is RightToLeft */
		SLATE_ARGUMENT(bool, FlipForRightToLeftFlowDirection)

		/** Invoked when the mouse is pressed in the widget. */
		SLATE_EVENT(FPointerEventHandler, OnMouseButtonDown)

		SLATE_SLOT_ARGUMENT(FSlot, Slots)
	SLATE_END_ARGS()

	/** Constructor */
	SAdvImage();

	/**
	 * Constructs this widget
	 *
	 * @param	InArgs	The declaration data for this widget
	 */
	void Construct(const FArguments& InArgs);

	/** See the ColorAndOpacity attribute */
	void SetColorAndOpacity(const TAttribute<FSlateColor>& InColorAndOpacity);

	/** See the ColorAndOpacity attribute */
	void SetColorAndOpacity(FLinearColor InColorAndOpacity);

	/** See the Image attribute */
	void SetImage(TAttribute<const FSlateBrush*> InImage);

protected:
	//~ Begin SPanel Interface
	virtual int32     OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	virtual FVector2D ComputeDesiredSize(float) const override;
	//~ End SPanel Interface

protected:
	/** The slate brush to draw for the image that we can invalidate. */
	FInvalidatableBrushAttribute Image;

	/** Color and opacity scale for this image */
	TSlateAttribute<FSlateColor> ColorAndOpacity;

	/** Flips the image if the localization's flow direction is RightToLeft */
	bool bFlipForRightToLeftFlowDirection;

	/** Invoked when the mouse is pressed in the image */
	FPointerEventHandler OnMouseButtonDownHandler;
};
