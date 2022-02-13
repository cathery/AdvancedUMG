// Copyright (c) 2022 cathery

#pragma once

#include "CoreMinimal.h"

#include "UMG/AdvPanelBase.h"

#include "AdvBorder.generated.h"

/**
 * A widget that draws a border that scales correctly with the UI.
 *
 * * Many Children
 * * Child Alignment
 * * Image (Border only)
 */
UCLASS(meta = (DisplayName = "Adv Border", ShortTooltip = "Draws a border that scales with the UI"))
class ADVANCEDUMG_API UAdvBorder : public UAdvPanelBase
{
	GENERATED_BODY()

public:
	/** Image to draw */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appearance")
	FSlateBrush Brush;

	/** How much the image should scale */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appearance", meta=( UIMin = "0", UIMax = "2", Delta = "0.01" ))
	float ImageScale;

public:
	/**  */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetBrush(const FSlateBrush& InBrush);

	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetImageScale(float InImageScale);

public:
	UAdvBorder();

	//~ Begin UWidget Interface
	virtual void SynchronizeProperties() override;
	//~ End UWidget Interface

	//~ Begin UVisual Interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	//~ End UVisual Interface

#if WITH_EDITOR
	//~ Begin UObject Interface
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	//~ End UObject Interface
#endif


protected:
	//~ Begin UWidget Interface
#if WITH_EDITOR
	virtual TSharedRef<SWidget> RebuildDesignWidget(TSharedRef<SWidget> Content) override
	{
		return Content;
	}
#endif
	//~ End UWidget Interface

protected:
	TSharedPtr<class SAdvBorder> MyBorder;

private:
	virtual TSharedPtr<SAdvPanel> GetSlateAdvPanel() const override;
	virtual void MakeSlateWidget() override;
};
