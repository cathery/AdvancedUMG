// Copyright (c) 2022 cathery

#pragma once

#include "CoreMinimal.h"

#include "Slate/SAdvPanel.h"

// Basic container widget that lets you define padding for child elements.
class SAdvPanelPadding : public SAdvPanel
{
	SLATE_DECLARE_WIDGET_API(SAdvPanelPadding, SAdvPanel, ADVANCEDUMG_API)

public:
	SLATE_BEGIN_ARGS(SAdvPanelPadding)
			: _Padding(FMargin(0.0f))
		{
			_Visibility = EVisibility::SelfHitTestInvisible;
		}

		SLATE_SLOT_ARGUMENT(FSlot, Slots)

		SLATE_ATTRIBUTE(FMargin, Padding)
	SLATE_END_ARGS()

	/** Constructor */
	SAdvPanelPadding();

	/**
	 * Construct this widget
	 *
	 * @param	InArgs	The declaration data for this widget
	 */
	void Construct(const FArguments& InArgs);

	void SetPadding(const TAttribute<FMargin>& InPadding);

protected:
	//~ Begin SPanel Interface
	virtual void      OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const override;
	virtual FVector2D ComputeDesiredSize(float) const override;
	//~ End SPanel Interface

protected:
	/** The panel's padding. */
	TSlateAttribute<FMargin> Padding;
};
