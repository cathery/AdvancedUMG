// Copyright (c) 2022 cathery

#pragma once

#include "CoreMinimal.h"

#include "UMG/AdvButtonBase.h"
#include "Slate/AdvButtonSimpleStyle.h"

#include "AdvButtonSimple.generated.h"

/**
 * A simple button with only one background image. The button will nudge down when pressed.
 *
 * * Many Children
 * * Child Alignment
 * * Press, Click, and Hover events
 */
UCLASS(meta = (DisplayName = "Adv Simple Button", ShortTooltip = "A simple button that will nudge down when pressed"))
class ADVANCEDUMG_API UAdvButtonSimple : public UAdvBaseButton
{
	GENERATED_BODY()

public:
	/** Button's appearance */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance", meta=(ShowOnlyInnerProperties))
	FAdvButtonSimpleStyle Style;

public:
	/** Sets the button's style */
	UFUNCTION(BlueprintCallable, Category = "Adv Spring Button|Appearance")
	void SetStyle(const FAdvButtonSimpleStyle& InStyle);

public:
	UAdvButtonSimple();

	//~ Begin UWidget Interface
	virtual void SynchronizeProperties() override;
	//~ End UWidget Interface

	//~ Begin UVisual Interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	//~ End UVisual Interface

protected:
	TSharedPtr<class SAdvButtonSimple> MyButton;

private:
	virtual void MakeSlateWidget() override;

	virtual TSharedPtr<SAdvButtonBase> GetSlateAdvButton() const override;
};
