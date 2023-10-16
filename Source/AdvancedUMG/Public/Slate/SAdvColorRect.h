// Copyright (c) 2022 cathery

#pragma once

#include "CoreMinimal.h"

#include "Slate/SAdvPanel.h"

// A widget that paints a solid color background with an optional outline
class SAdvColorRect : public SAdvPanel
{
	SLATE_DECLARE_WIDGET_API(SAdvColorRect, SAdvPanel, ADVANCEDUMG_API)

public:
	SLATE_BEGIN_ARGS(SAdvColorRect)
			: _ColorAndOpacity(FLinearColor::White)
			, _OutlineColorAndOpacity(FLinearColor::Black)
			, _OutlineThickness(0.0f)
		{
		}

		/** Color and opacity for the solid color */
		SLATE_ATTRIBUTE(FSlateColor, ColorAndOpacity)

		/** Color and opacity for the outline */
		SLATE_ATTRIBUTE(FSlateColor, OutlineColorAndOpacity)

		/** Outline thickness */
		SLATE_ATTRIBUTE(FMargin, OutlineThickness)

		/** Invoked when the mouse is pressed in the widget. */
		SLATE_EVENT(FPointerEventHandler, OnMouseButtonDown)

		SLATE_SLOT_ARGUMENT(FSlot, Slots)
	SLATE_END_ARGS()

	/** Constructor */
	SAdvColorRect();

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

	/** See the ColorAndOpacity attribute */
	void SetOutlineColorAndOpacity(const TAttribute<FSlateColor>& InOutlineColorAndOpacity);

	/** See the ColorAndOpacity attribute */
	void SetOutlineColorAndOpacity(FLinearColor InOutlineColorAndOpacity);

	/** See the OutlineThickness attribute */
	void SetOutlineThickness(const TAttribute<FMargin> InOutlineThickness);

protected:
	//~ Begin SPanel Interface
	virtual int32     OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	virtual FVector2D ComputeDesiredSize(float) const override;
	//~ End SPanel Interface

protected:
	/** The slate brush for the solid color */
	static const FSlateBrush Brush;

	/** Color and opacity for the solid color */
	TSlateAttribute<FSlateColor> ColorAndOpacity;

	/** Color and opacity for the outline */
	TSlateAttribute<FSlateColor> OutlineColorAndOpacity;

	/** Outline thickness */
	TSlateAttribute<FMargin> OutlineThickness;

	/** Invoked when the mouse is pressed in the widget. */
	FPointerEventHandler OnMouseButtonDownHandler;
};
