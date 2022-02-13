// Copyright (c) 2022 cathery

#pragma once

#include "CoreMinimal.h"

#include "Slate/SAdvPanel.h"

// Basic container widget that lets you define padding for child elements.
class ADVANCEDUMG_API SAdvPanelPadding : public SAdvPanel
{
public:
	SLATE_BEGIN_ARGS(SAdvPanelPadding)
			: _Padding(FMargin(0.0f))
		{
			_Visibility = EVisibility::SelfHitTestInvisible;
		}

		SLATE_SUPPORTS_SLOT(SAdvPanel::FSlot)

		SLATE_ATTRIBUTE(FMargin, Padding)
	SLATE_END_ARGS()

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
	TAttribute<FMargin> Padding;
};
