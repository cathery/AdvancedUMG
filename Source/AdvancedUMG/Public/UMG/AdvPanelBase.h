// Copyright (c) 2022 cathery

#pragma once

#include "CoreMinimal.h"
#include "Components/PanelWidget.h"

#include "AdvPanelBase.generated.h"

/** The base class for all UMG Adv panel widgets. */
UCLASS(Abstract)
class ADVANCEDUMG_API UAdvPanelBase : public UPanelWidget
{
	GENERATED_BODY()

public:
	FNoReplyPointerEventHandler MouseEnterEvent;

	FNoReplyPointerEventHandler MouseMoveEvent;

	FNoReplyPointerEventHandler MouseWheelEvent;

	FNoReplyPointerEventHandler MouseButtonDownEvent;

	FNoReplyPointerEventHandler MouseButtonUpEvent;

	FSimpleNoReplyPointerEventHandler MouseLeaveEvent;

	UAdvPanelBase();

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
	//~ Begin UWidget Interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	//~ End UWidget Interface

private:
	virtual TSharedPtr<class SAdvPanel> GetSlateAdvPanel() const;
	virtual void MakeSlateWidget();
	void BindSlateWidgetEvents();

	void OnSlateMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

	FReply OnSlateMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

	FReply OnSlateMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

	FReply OnSlateMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

	UFUNCTION()
	void OnSlateAdvMouseWheelEvent(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

	void OnSlateMouseLeave(const FPointerEvent& InMouseEvent);
};
