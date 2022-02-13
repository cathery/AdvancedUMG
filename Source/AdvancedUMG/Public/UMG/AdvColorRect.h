// Copyright (c) 2022 cathery

#pragma once

#include "CoreMinimal.h"

#include "UMG/AdvPanelBase.h"

#include "AdvColorRect.generated.h"

class USlateBrushAsset;

/**
 * A widget that paints a solid color background with an optional outline.
 *
 * * Many Children
 * * Child Alignment
 * * Solid Color
 */
UCLASS(meta = (DisplayName = "Adv Color Rect", ShortTooltip = "Paints a solid background with an optional outline"))
class ADVANCEDUMG_API UAdvColorRect : public UAdvPanelBase
{
	GENERATED_BODY()

public:
	/** Color and opacity for the solid color */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appearance", meta=( sRGB="true"))
	FLinearColor ColorAndOpacity;

	/** Color and opacity for the outline */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appearance", meta=( sRGB="true"))
	FLinearColor OutlineColorAndOpacity;

	/** Outline thickness */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appearance")
	FMargin OutlineThickness;

	/** A bindable delegate for the ColorAndOpacity. */
	UPROPERTY()
	FGetLinearColor ColorAndOpacityDelegate;

	/** A bindable delegate for the OutlineColorAndOpacity. */
	UPROPERTY()
	FGetLinearColor OutlineColorAndOpacityDelegate;

public:
	UPROPERTY(EditAnywhere, Category=Events, meta=( IsBindableEvent="True" ))
	FOnPointerEvent OnMouseButtonDownEvent;

public:
	/**  */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetColorAndOpacity(FLinearColor InColorAndOpacity);

	/**  */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetOpacity(float InOpacity);

	/**  */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetOutlineColorAndOpacity(FLinearColor InOutlineColorAndOpacity);

	/**  */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetOutlineOpacity(float InOutlineOpacity);

	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetOutlineThickness(const FMargin& InOutlineThickness);

public:
	UAdvColorRect();

	//~ Begin UWidget Interface
	virtual void SynchronizeProperties() override;
	//~ End UWidget Interface

	//~ Begin UVisual Interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	//~ End UVisual Interface

protected:
	//
	FReply HandleMouseButtonDown(const FGeometry& Geometry, const FPointerEvent& MouseEvent) const;

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
	TSharedPtr<class SAdvColorRect> MyRect;

private:
	virtual TSharedPtr<SAdvPanel> GetSlateAdvPanel() const override;
	virtual void MakeSlateWidget() override;

protected:
	PROPERTY_BINDING_IMPLEMENTATION(FSlateColor, ColorAndOpacity);
	PROPERTY_BINDING_IMPLEMENTATION(FSlateColor, OutlineColorAndOpacity);
};
