// Copyright (c) 2022 cathery

#pragma once

#include "CoreMinimal.h"

#include "Slate/AdvButtonGenericStyle.h"
#include "UMG/AdvButtonBase.h"

#include "AdvButtonGeneric.generated.h"

/**
 * A generic button with normal, hovered, and pressed states. Most similar to Button.
 *
 * * Many Children
 * * Child Alignment
 * * Press, Click, and Hover events 
 */
UCLASS(meta = (DisplayName = "Adv Generic Button", ShortTooltip = "A generic button"))
class ADVANCEDUMG_API UAdvButtonGeneric : public UAdvBaseButton
{
	GENERATED_BODY()

public:
	/** The button style used at runtime */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
	FAdvButtonGenericStyle Style;

	/** The color multiplier for the button content */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appearance", meta=( sRGB="true" ))
	FLinearColor ColorAndOpacity;

	/** The color multiplier for the button background */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appearance", meta=( sRGB="true" ))
	FLinearColor BackgroundColor;

public:
	/** Sets the color multiplier for the button background */
	UFUNCTION(BlueprintCallable, Category = "Adv Generic Button|Appearance")
	void SetStyle(const FAdvButtonGenericStyle& InStyle);

	/** Sets the color multiplier for the button content */
	UFUNCTION(BlueprintCallable, Category = "Adv Generic Button|Appearance")
	void SetColorAndOpacity(FLinearColor InColorAndOpacity);

	/** Sets the color multiplier for the button background */
	UFUNCTION(BlueprintCallable, Category = "Adv Generic Button|Appearance")
	void SetBackgroundColor(FLinearColor InBackgroundColor);

public:
	UAdvButtonGeneric();

	//~ Begin UWidget Interface
	virtual void SynchronizeProperties() override;
	//~ End UWidget Interface

	//~ Begin UVisual Interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	//~ End UVisual Interface

protected:
	TSharedPtr<class SAdvButtonGeneric> MyButton;

private:
	virtual void MakeSlateWidget() override;

	virtual TSharedPtr<SAdvButtonBase> GetSlateAdvButton() const override;
};
