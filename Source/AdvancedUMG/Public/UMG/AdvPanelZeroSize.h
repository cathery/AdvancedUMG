// Copyright (c) 2022 cathery

#pragma once

#include "CoreMinimal.h"

#include "UMG/AdvPanelBase.h"

#include "AdvPanelZeroSize.generated.h"

/**
 * (Advanced) A panel that always reports a desired size of zero.
 * Useful for cases where your parent must size to content but you don't want this widget's content to be taken into account.
 *
 * * Many Children
 * * Padding
 * * Child Alignment
 */
UCLASS(meta = (DisplayName = "Adv Zero Size Panel", ShortTooltip = "A panel that reports a desired size of zero"))
class ADVANCEDUMG_API UAdvPanelZeroSize : public UAdvPanelBase
{
	GENERATED_BODY()

public:
	//~ Begin UVisual Interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	//~ End UVisual Interface

protected:
	TSharedPtr<class SAdvPanelZeroSize> MyPanel;

private:
	virtual TSharedPtr<SAdvPanel> GetSlateAdvPanel() const override;
	virtual void MakeSlateWidget() override;
};
