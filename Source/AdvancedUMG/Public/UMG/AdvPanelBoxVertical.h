// Copyright (c) 2022 cathery

#pragma once

#include "CoreMinimal.h"
#include "Components/PanelWidget.h"

#include "AdvPanelBoxVertical.generated.h"

/**
 * A vertical box widget is a layout panel allowing child widgets to be automatically laid out
 * vertically.
 *
 * * Many Children
 * * Flows Vertical
 */
UCLASS(meta = (DisplayName = "Adv Vertical Box", ShortTooltip = "A layout panel for automatically laying child widgets out vertically"))
class ADVANCEDUMG_API UAdvPanelBoxVertical : public UPanelWidget
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
	UAdvPanelBoxVertical();

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
