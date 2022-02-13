// Copyright (c) 2022 cathery

#pragma once

#include "CoreMinimal.h"
#include "Components/PanelSlot.h"

#include "Slate/SAdvPanel.h"

#include "AdvPanelSlot.generated.h"

/**
 * The Slot for the AdvPanel, contains the individual child alignment options 
 */
UCLASS(meta = (DisplayName = "Adv Panel Slot"))
class ADVANCEDUMG_API UAdvPanelSlot : public UPanelSlot
{
	GENERATED_BODY()

public:
	/** The padding area between the slot and the content it contains. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Layout|Adv Panel Slot")
	FMargin Padding;

	/** The alignment of the object horizontally. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Layout|Adv Panel Slot")
	TEnumAsByte<EHorizontalAlignment> HorizontalAlignment;

	/** The alignment of the object vertically. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Layout|Adv Panel Slot")
	TEnumAsByte<EVerticalAlignment> VerticalAlignment;

public:
	UAdvPanelSlot();

	UFUNCTION(BlueprintCallable, Category = "Layout|Adv Panel Slot")
	void SetPadding(FMargin InPadding);

	UFUNCTION(BlueprintCallable, Category = "Layout|Adv Panel Slot")
	void SetHorizontalAlignment(EHorizontalAlignment InHorizontalAlignment);

	UFUNCTION(BlueprintCallable, Category = "Layout|Adv Panel Slot")
	void SetVerticalAlignment(EVerticalAlignment InVerticalAlignment);

public:
	//~ Begin UPanelSlot Interface
	virtual void SynchronizeProperties() override;
	//~ End UPanelSlot Interface

	/** Builds the underlying slot for the slate padding panel. */
	void BuildSlot(TSharedRef<SAdvPanel> Container);

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

protected:
	SAdvPanel::FSlot* Slot;
};
