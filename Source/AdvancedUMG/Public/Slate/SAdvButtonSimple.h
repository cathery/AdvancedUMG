// Copyright (c) 2022 cathery

#pragma once

#include "CoreMinimal.h"
#include "AdvSlateStyle.h"

#include "Slate/AdvButtonSimpleStyle.h"
#include "Slate/SAdvButtonBase.h"

// A simple button with only one background image. The button will nudge down when pressed.
class SAdvButtonSimple : public SAdvButtonBase
{
	SLATE_DECLARE_WIDGET_API(SAdvButtonSimple, SAdvButtonBase, ADVANCEDUMG_API)

public:
	SLATE_BEGIN_ARGS(SAdvButtonSimple)
			: _ClickMethod(EButtonClickMethod::DownAndUp)
			, _TouchMethod(EButtonTouchMethod::DownAndUp)
			, _PressMethod(EButtonPressMethod::DownAndUp)
			, _IsFocusable(true)
			, _Style(&FAdvSlateStyle::Get().GetWidgetStyle<FAdvButtonSimpleStyle>("DefaultSimpleButton"))
		{
		}

		/** Sets the rules to use for determining whether the button was clicked.  This is an advanced setting and generally should be left as the default. */
		SLATE_ARGUMENT(EButtonClickMethod::Type, ClickMethod)

		/** How should the button be clicked with touch events? */
		SLATE_ARGUMENT(EButtonTouchMethod::Type, TouchMethod)

		/** How should the button be clicked with keyboard/controller button events? */
		SLATE_ARGUMENT(EButtonPressMethod::Type, PressMethod)

		/** Sometimes a button should only be mouse-clickable and never keyboard focusable. */
		SLATE_ARGUMENT(bool, IsFocusable)

		/** Called when the button is clicked */
		SLATE_EVENT(FOnClicked, OnClicked)

		/** Called when the button is pressed */
		SLATE_EVENT(FSimpleDelegate, OnPressed)

		/** Called when the button is released */
		SLATE_EVENT(FSimpleDelegate, OnReleased)

		/** Called when the button is hovered over */
		SLATE_EVENT(FSimpleDelegate, OnHovered)

		/** Called when the button is no longer hovered over */
		SLATE_EVENT(FSimpleDelegate, OnUnhovered)

		/** The visual style of the button */
		SLATE_STYLE_ARGUMENT(FAdvButtonSimpleStyle, Style)

		SLATE_SLOT_ARGUMENT(FSlot, Slots)
	SLATE_END_ARGS()

public:
	/** Default constructor. */
	SAdvButtonSimple();

	/**
	 * Construct this widget
	 *
	 * @param	InArgs	The declaration data for this widget
	 */
	void Construct(const FArguments& InArgs);

	void SetStyle(const FAdvButtonSimpleStyle* InStyle);

protected:
	/** The visual style of the button */
	const FAdvButtonSimpleStyle* Style;

protected:
	//~ Begin SAdvBaseButton Interface
	virtual void OnClick() override;
	virtual void OnPress() override;
	virtual void OnHover() override;
	//~ End SAdvBaseButton Interface

	//~ Begin SPanel Interface
	virtual void OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const override;
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	virtual FVector2D ComputeDesiredSize(float) const override;
	//~ End SPanel Interface
};
