// Copyright (c) 2022 cathery

#pragma once

#include "CoreMinimal.h"

#include "UMG/AdvPanelBase.h"

#include "AdvPanelPadding.generated.h"

/**
 * A panel that lets you define padding for child elements.
 *
 * * Many Children
 * * Padding
 * * Child Alignment
 */
UCLASS(meta = (DisplayName = "Adv Padding Panel", ShortTooltip = "A panel that lets you define padding for child elements"))
class ADVANCEDUMG_API UAdvPanelPadding : public UAdvPanelBase
{
	GENERATED_BODY()

public:
	/** The padding area between the panel and the content it contains. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Content")
	FMargin Padding;

public:
	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetPadding(FMargin InPadding);

public:
	//~ Begin UWidget Interface
	virtual void SynchronizeProperties() override;
	//~ End UWidget Interface

	//~ Begin UVisual Interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	//~ End UVisual Interface

protected:
	TSharedPtr<class SAdvPanelPadding> MyPanel;

private:
	virtual TSharedPtr<SAdvPanel> GetSlateAdvPanel() const override;
	virtual void MakeSlateWidget() override;
};
