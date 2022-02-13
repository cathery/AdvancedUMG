// Copyright (c) 2022 cathery

#pragma once

#include "CoreMinimal.h"

#include "UMG/AdvPanelBase.h"

#include "AdvPanel.generated.h"

/**
 * The most basic panel that doesn't do anything.
 *
 * * Many Children
 * * Child Alignment
 */
UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Adv Panel", ShortTooltip = "A basic panel that doesn't do anything"))
class ADVANCEDUMG_API UAdvPanel final : public UAdvPanelBase
{
	GENERATED_BODY()

public:
	//~ Begin UVisual Interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	//~ End UVisual Interface

protected:
	TSharedPtr<class SAdvPanel> MyPanel;

private:
	virtual TSharedPtr<SAdvPanel> GetSlateAdvPanel() const override;
	virtual void MakeSlateWidget() override;
};
