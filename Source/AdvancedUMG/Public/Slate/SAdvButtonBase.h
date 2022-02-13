// Copyright (c) 2022 cathery

#pragma once

#include "CoreMinimal.h"

#include "Slate/SAdvPanel.h"

/**
 * The base for all Adv Buttons. It has all the functionality of a button without any of the visuals.
 */
class ADVANCEDUMG_API SAdvButtonBase : public SAdvPanel
{
public:
	SLATE_BEGIN_ARGS(SAdvButtonBase)
			: _ClickMethod(EButtonClickMethod::DownAndUp)
			, _TouchMethod(EButtonTouchMethod::DownAndUp)
			, _PressMethod(EButtonPressMethod::DownAndUp)
			, _IsFocusable(false)
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

		SLATE_SUPPORTS_SLOT(SAdvPanel::FSlot)
	SLATE_END_ARGS()

public:
	/** Default constructor. */
	SAdvButtonBase();

	/**
	 * Construct this widget
	 *
	 * @param	InArgs	The declaration data for this widget
	 */
	void Construct(const FArguments& InArgs);

	/**
	 * Returns true if this button is currently pressed
	 *
	 * @return	True if pressed, otherwise false
	 */
	virtual bool IsPressed() const
	{
		return bIsPressed;
	}

	/** See OnClicked event */
	void SetOnClicked(FOnClicked InOnClicked);

	void SetOnPressed(FSimpleDelegate InOnPressed);

	void SetOnReleased(FSimpleDelegate InOnReleased);

	/** Set OnHovered event */
	void SetOnHovered(FSimpleDelegate InOnHovered);

	/** Set OnUnhovered event */
	void SetOnUnhovered(FSimpleDelegate InOnUnhovered);

	void SetClickMethod(EButtonClickMethod::Type InClickMethod);
	void SetTouchMethod(EButtonTouchMethod::Type InTouchMethod);
	void SetPressMethod(EButtonPressMethod::Type InPressMethod);

public:
	//~ Begin SPanel Interface
	virtual bool   SupportsKeyboardFocus() const override;
	virtual void   OnFocusLost(const FFocusEvent& InFocusEvent) override;
	virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply OnKeyUp(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonDoubleClick(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void   OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void   OnMouseLeave(const FPointerEvent& MouseEvent) override;
	virtual void   OnMouseCaptureLost(const FCaptureLostEvent& CaptureLostEvent) override;
	virtual bool   IsInteractable() const override;
	virtual bool   ComputeVolatility() const override;
#if WITH_ACCESSIBILITY
	virtual TSharedRef<FSlateAccessibleWidget> CreateAccessibleWidget() override;
#endif
	//~ End SPanel Interface

protected:
	/** The location in screenspace the button was pressed */
	FVector2D PressedScreenSpacePosition;

	/** Sets whether a click should be triggered on mouse down, mouse up, or that both a mouse down and up are required. */
	TEnumAsByte<EButtonClickMethod::Type> ClickMethod;

	/** How should the button be clicked with touch events? */
	TEnumAsByte<EButtonTouchMethod::Type> TouchMethod;

	/** How should the button be clicked with keyboard/controller button events? */
	TEnumAsByte<EButtonPressMethod::Type> PressMethod;

	/** Can this button be focused? */
	uint8 bIsFocusable:1;

	/** True if this button is currently in a pressed state */
	uint8 bIsPressed:1;

	/** The delegate to execute when the button is clicked */
	FOnClicked OnClicked;

	/** The delegate to execute when the button is pressed */
	FSimpleDelegate OnPressed;

	/** The delegate to execute when the button is released */
	FSimpleDelegate OnReleased;

	FSimpleDelegate OnHovered;

	FSimpleDelegate OnUnhovered;

	/** Click the button. */
	FReply Click();

	/** Press the button. */
	void Press();

	/** Release the button. */
	void Release();

	/** Hover over the button. */
	void Hover();

	/** Unhover the button. */
	void Unhover();

	/** Overridable virtual function for when button has been clicked */
	virtual void OnClick();

	/** Overridable virtual function for when button has been pressed */
	virtual void OnPress();

	/** Overridable virtual function for when button has been released */
	virtual void OnRelease();

	/** Overridable virtual function for when button has been hovered over */
	virtual void OnHover();

	/** Overridable virtual function for when button has been unhovered */
	virtual void OnUnhover();

	/** Utility function to translate other input click methods to regular ones. */
	TEnumAsByte<EButtonClickMethod::Type> GetClickMethodFromInputType(const FPointerEvent& MouseEvent) const;

	/** Utility function to determine if the incoming mouse event is for a precise tap or click */
	bool IsPreciseTapOrClick(const FPointerEvent& MouseEvent) const;

protected:
	//~ Begin SPanel Interface
	virtual FVector2D ComputeDesiredSize(float) const override;
	//~ End SPanel Interface

private:
#if WITH_ACCESSIBILITY
	// Allow the accessible button to "click" this button
	friend class FSlateAccessibleButton;
#endif
};
