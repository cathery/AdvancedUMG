// Copyright (c) 2022 cathery

#pragma once

#include "CoreMinimal.h"
#include "Components/PanelSlot.h"
#include "Components/SlateWrapperTypes.h"

#include "Slate/SAdvPanelBox.h"

#include "AdvPanelBoxSlot.generated.h"

/**
 * The Slot for the AdvBoxHorizontal and AdvBoxVertical, contains the individual child alignment options 
 */
UCLASS(meta = (DisplayName = "Adv Box Panel Slot"))
class ADVANCEDUMG_API UAdvPanelBoxSlot : public UPanelSlot
{
	GENERATED_BODY()

public:
	/** The padding area between the slot and the content it contains. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Layout|Adv Box Panel Slot")
	FMargin Padding;

	/** How much space this slot should occupy in the direction of the panel. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Layout|Adv Box Panel Slot")
	FSlateChildSize Size;

	/** The alignment of the object horizontally. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Layout|Adv Box Panel Slot")
	TEnumAsByte<EHorizontalAlignment> HorizontalAlignment;

	/** The alignment of the object vertically. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Layout|Adv Box Panel Slot")
	TEnumAsByte<EVerticalAlignment> VerticalAlignment;

public:
	UAdvPanelBoxSlot();

	UFUNCTION(BlueprintCallable, Category = "Layout|Adv Box Panel Slot")
	void SetSize(FSlateChildSize InSize);

	UFUNCTION(BlueprintCallable, Category = "Layout|Adv Box Panel Slot")
	void SetPadding(FMargin InPadding);

	UFUNCTION(BlueprintCallable, Category = "Layout|Adv Box Panel Slot")
	void SetHorizontalAlignment(EHorizontalAlignment InHorizontalAlignment);

	UFUNCTION(BlueprintCallable, Category = "Layout|Adv Box Panel Slot")
	void SetVerticalAlignment(EVerticalAlignment InVerticalAlignment);
public:
	// UPanelSlot interface
	virtual void SynchronizeProperties() override;
	// End of UPanelSlot interface

	/** Builds the underlying slot for the slate padding panel. */
	void BuildSlot(TSharedRef<SAdvPanelBox> Container);

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

private:
	SAdvPanelBox::FSlot* Slot;
};
