// Copyright (c) 2022 cathery

#pragma once

#include "CoreMinimal.h"

#include "Slate/SAdvBottomSheet.h"
#include "UMG/AdvPanelSlot.h"

#include "AdvBottomSheetSlot.generated.h"

/**
 * The Slot for the AdvBottomSheet 
 */
UCLASS(meta = (DisplayName = "Adv Bottom Sheet Slot"))
class ADVANCEDUMG_API UAdvBottomSheetSlot : public UAdvPanelSlot
{
	GENERATED_BODY()

public:
	/** Should this widget stay on top of the sheet at all times? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Layout|Adv Bottom Sheet Slot")
	bool bIsPersistent;

public:
	UAdvBottomSheetSlot();

	UFUNCTION(BlueprintCallable, Category = "Layout|Adv Bottom Sheet Slot")
	void SetIsPersistent(bool bInIsPersistent);

public:
	//~ Begin UPanelSlot Interface
	virtual void SynchronizeProperties() override;
	//~ End UPanelSlot Interface

	/** Builds the underlying slot for the slate panel. */
	void BuildSlot(TSharedRef<SAdvBottomSheet> Container);

	/** Destroys the underlying slot for the slate panel. */
	void RemoveSlot(TSharedRef<SAdvBottomSheet> Container);
};
