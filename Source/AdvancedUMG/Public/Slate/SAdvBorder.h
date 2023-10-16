// Copyright (c) 2022 cathery

#pragma once

#include "CoreMinimal.h"

#include "Slate/SAdvPanel.h"

/**
 * A widget that draws a border that scales correctly with the UI.
 *
 * * Many Children
 * * Child Alignment
 * * Image (Border only)
 */
class SAdvBorder : public SAdvPanel
{
	SLATE_DECLARE_WIDGET_API(SAdvBorder, SAdvPanel, ADVANCEDUMG_API)

public:
	SLATE_BEGIN_ARGS(SAdvBorder)
			: _Image(*FCoreStyle::Get().GetDefaultBrush())
			, _ImageScale(1.0f)
		{
		}

		/** Image resource */
		SLATE_ARGUMENT(FSlateBrush, Image)
		SLATE_ARGUMENT(float, ImageScale)

		SLATE_SLOT_ARGUMENT(FSlot, Slots)
	SLATE_END_ARGS()

	/** Constructor */
	SAdvBorder();

	/**
	 * Constructs this widget
	 *
	 * @param	InArgs	The declaration data for this widget
	 */
	void Construct(const FArguments& InArgs);
	
	/** See the Image attribute */
	void SetImage(const FSlateBrush& InImage);

	void SetImageScale(float InImageScale);

protected:
	//~ Begin SPanel Interface
	virtual int32     OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	virtual FVector2D ComputeDesiredSize(float) const override;
	//~ End SPanel Interface

protected:
	/** The slate brush to draw for the image that we can invalidate. */
	FSlateBrush Image;

	/** How much the image should scale */
	float ImageScale;
};
