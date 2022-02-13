// Copyright (c) 2022 cathery

#pragma once

#include "CoreMinimal.h"

#include "UMG/AdvPanelBase.h"

#include "AdvBottomSheet.generated.h"

/**
 * A sheet panel (todo)
 *
 * * Many Children
 * * Padding
 * * Child Alignment
 */
UCLASS(meta = (DisplayName = "Adv Bottom Sheet", ShortTooltip = "A bottom sheet"))
class ADVANCEDUMG_API UAdvBottomSheet : public UAdvPanelBase
{
	GENERATED_BODY()

public:
	/** Background image to draw */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appearance")
	FSlateBrush Background;

	/** How much the mouse/finger should move before we register it as a drag gesture */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appearance", meta=(ClampMin=0.0f))
	float DragThreshold;

	/** How much the mouse/finger should move before the sheet recognizes it as an extend/retract gesture */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appearance", meta=(ClampMin=0.0f))
	float MoveThreshold;

	/** How fast the sheet animation should catch up with the movement */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appearance", meta=(ClampMin=0.0f))
	float SheetInterpSpeed;

	/** How fast the scroll animation should catch up with the movement */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appearance", meta=(ClampMin=0.0f))
	float ScrollInterpSpeed;

	/** How much should the sheet move from scrolling */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appearance", meta=(ClampMin=0.0f))
	float SheetSpeedMultiplier;

	/** How much should the content move from scrolling */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appearance", meta=(ClampMin=0.0f))
	float ScrollSpeedMultiplier;

	// The sheet will try to align with the closest of these heights (in % of the total space)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appearance")
	TArray<float> SheetTargetAreas;

#if WITH_EDITORONLY_DATA

	UPROPERTY(EditAnywhere, AdvancedDisplay, Transient, Category = "Appearance")
	float SheetOffset;

	UPROPERTY(EditAnywhere, AdvancedDisplay, Transient, Category = "Appearance")
	float ScrollOffset;

#endif


public:
	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetBackground(const FSlateBrush& InBackground);

	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetDragThreshold(float InDragThreshold);
	
	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetMoveThreshold(float InMoveThreshold);

	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetSheetInterpSpeed(float InSheetInterpSpeed);

	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetScrollInterpSpeed(float InScrollInterpSpeed);

	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetSheetSpeedMultiplier(float InSheetSpeedMultiplier);

	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetScrollSpeedMultiplier(float InScrollSpeedMultiplier);

	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetSheetTargetAreas(const TArray<float>& InSheetTargetAreas);

public:
	UAdvBottomSheet();

	//~ Begin UWidget Interface
	virtual void SynchronizeProperties() override;
	//~ End UWidget Interface

	//~ Begin UVisual Interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	//~ End UVisual Interface

protected:
	//~ Begin UPanelWidget Interface
	virtual UClass* GetSlotClass() const override;
	virtual void    OnSlotAdded(UPanelSlot* InSlot) override;
	virtual void    OnSlotRemoved(UPanelSlot* InSlot) override;
	//~ End UPanelWidget Interface

protected:
	//~ Begin UWidget Interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	//~ End UWidget Interface

protected:
	TSharedPtr<class SAdvBottomSheet> MySheet;

private:
	virtual TSharedPtr<SAdvPanel> GetSlateAdvPanel() const override final;
	virtual TSharedPtr<SAdvBottomSheet> GetSlateAdvBottomSheet() const;
	virtual void MakeSlateWidget() override;
};
