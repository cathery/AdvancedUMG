// Copyright (c) 2022 cathery

#pragma once

#include "CoreMinimal.h"

#include "Slate/SAdvPanel.h"

/**
 * (Advanced) A panel that always reports a desired size of zero.
 * Useful for cases where your parent must size to content but you don't want this widget's content to be taken into account.
 *
 * * Many Children
 * * Padding
 * * Child Alignment
 */
class ADVANCEDUMG_API SAdvPanelZeroSize : public SAdvPanel
{
public:
	SLATE_BEGIN_ARGS(SAdvPanelZeroSize)
		{
			_Visibility = EVisibility::SelfHitTestInvisible;
		}

		SLATE_SUPPORTS_SLOT(SAdvPanel::FSlot)
	SLATE_END_ARGS()

	/**
	 * Construct this widget
	 *
	 * @param	InArgs	The declaration data for this widget
	 */
	void Construct(const FArguments& InArgs);

protected:
	//~ Begin SPanel Interface
	virtual FVector2D ComputeDesiredSize(float LayoutScaleMultiplier) const override;
	//~ End SPanel Interface
};
