// Copyright (c) 2022 cathery

#pragma once

#include "CoreMinimal.h"
#include "AdvSlateStyle.h"

#include "Slate/AdvButtonGenericStyle.h"
#include "Slate/SAdvButtonBase.h"

// A generic button widget with normal, hovered, and pressed states. Most similar to SButton.
class ADVANCEDUMG_API SAdvButtonGeneric : public SAdvButtonBase
{
public:
	SLATE_BEGIN_ARGS(SAdvButtonGeneric)
			: _ClickMethod(EButtonClickMethod::DownAndUp)
			, _TouchMethod(EButtonTouchMethod::DownAndUp)
			, _PressMethod(EButtonPressMethod::DownAndUp)
			, _IsFocusable(true)
			, _Style(&FAdvSlateStyle::Get().GetWidgetStyle<FAdvButtonGenericStyle>("DefaultGenericButton"))
			, _ColorAndOpacity(FLinearColor::White)
			, _BackgroundColor(FLinearColor::White)
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
		SLATE_STYLE_ARGUMENT(FAdvButtonGenericStyle, Style)

		/** Color for the child widgets */
		SLATE_ATTRIBUTE(FLinearColor, ColorAndOpacity)

		/** Color for the button itself */
		SLATE_ATTRIBUTE(FLinearColor, BackgroundColor)

		SLATE_SUPPORTS_SLOT(SAdvPanel::FSlot)
	SLATE_END_ARGS()

public:
	/** Default constructor. */
	SAdvButtonGeneric();

	/**
	 * Construct this widget
	 *
	 * @param	InArgs	The declaration data for this widget
	 */
	void Construct(const FArguments& InArgs);

	/** @return An image that represents this button's border*/
	virtual const FSlateBrush* GetBorder() const;

	/** See ButtonStyle attribute */
	void SetButtonStyle(const FAdvButtonGenericStyle* ButtonStyle);

	void SetColorAndOpacity(const TAttribute<FLinearColor>& InColorAndOpacity);

	void SetBorderBackgroundColor(const TAttribute<FLinearColor>& InBackgroundColor);


protected:
	/** @return combines the user-specified margin and the button's internal margin. */
	FMargin GetCombinedPadding() const;

	/** @return True if the disabled effect should be shown. */
	bool GetShowDisabledEffect() const;

	/** Style resource for the button */
	const FAdvButtonGenericStyle* Style;

	/** Color for the child widgets */
	TAttribute<FLinearColor> ColorAndOpacity;

	/** Color for the button itself */
	TAttribute<FLinearColor> BackgroundColor;

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
