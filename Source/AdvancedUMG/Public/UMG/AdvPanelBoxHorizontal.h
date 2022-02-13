// Copyright (c) 2022 cathery

#pragma once

#include "CoreMinimal.h"
#include "Components/PanelWidget.h"

#include "AdvPanelBoxHorizontal.generated.h"

/**
 * Allows widgets to be laid out in a flow horizontally.
 *
 * * Many Children
 * * Flow Horizontal
 */
UCLASS(meta = (DisplayName = "Adv Horizontal Box", ShortTooltip = "A layout panel for automatically laying child widgets out horizontally"))
class ADVANCEDUMG_API UAdvPanelBoxHorizontal : public UPanelWidget
{
	GENERATED_BODY()

public:
	/** The spacing between the child elements. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Content")
	float Spacing;

public:
	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetSpacing(float InSpacing);

public:
	UAdvPanelBoxHorizontal();

	//~ Begin UWidget Interface
	virtual void SynchronizeProperties() override;
	//~ End UWidget Interface

	//~ Begin UVisual Interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	//~ End UVisual Interface

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif

protected:
	//~ Begin UPanelWidget Interface
	virtual UClass* GetSlotClass() const override;
	virtual void    OnSlotAdded(UPanelSlot* InSlot) override;
	virtual void    OnSlotRemoved(UPanelSlot* InSlot) override;
	//~ End UPanelWidget Interface

protected:
	TSharedPtr<class SAdvPanelBox> MyBoxPanel;

protected:
	//~ Begin UWidget Interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	//~ End UWidget Interface
};
