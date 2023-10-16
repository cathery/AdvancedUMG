// Copyright (c) 2022 cathery

#include "Slate/SAdvButtonBase.h"

#include "Widgets/Accessibility/SlateAccessibleWidgets.h"

SLATE_IMPLEMENT_WIDGET(SAdvButtonBase)
void SAdvButtonBase::PrivateRegisterAttributes(FSlateAttributeInitializer& AttributeInitializer)
{
}

SAdvButtonBase::SAdvButtonBase()
	: PressedScreenSpacePosition(FVector2D::ZeroVector)
	, ClickMethod(EButtonClickMethod::DownAndUp)
	, TouchMethod(EButtonClickMethod::DownAndUp)
	, PressMethod(EButtonClickMethod::DownAndUp)
	, bIsFocusable(false)
	, bIsPressed(false)
{
#if WITH_ACCESSIBILITY
	AccessibleBehavior       = EAccessibleBehavior::Summary;
	bCanChildrenBeAccessible = false;
#endif
}

/**
 * Construct this widget
 *
 * @param	InArgs	The declaration data for this widget
 */
void SAdvButtonBase::Construct(const FArguments& InArgs)
{
	// Call the parent constructor with our slots
	Super::FArguments ParentArgs;
	ParentArgs._Slots = MoveTemp(const_cast<FArguments&>(InArgs)._Slots);
	Super::Construct(ParentArgs);

	PressedScreenSpacePosition = FVector2D::ZeroVector;

	ClickMethod = InArgs._ClickMethod;
	TouchMethod = InArgs._TouchMethod;
	PressMethod = InArgs._PressMethod;

	bIsFocusable = InArgs._IsFocusable;
	bIsPressed   = false;

	OnClicked     = InArgs._OnClicked;
	OnPressed     = InArgs._OnPressed;
	OnReleased    = InArgs._OnReleased;
	OnHovered     = InArgs._OnHovered;
	OnUnhovered   = InArgs._OnUnhovered;
}

bool SAdvButtonBase::SupportsKeyboardFocus() const
{
	// Buttons are focusable by default
	return bIsFocusable;
}

void SAdvButtonBase::OnFocusLost(const FFocusEvent& InFocusEvent)
{
	SAdvPanel::OnFocusLost(InFocusEvent);
	Release();
}

FReply SAdvButtonBase::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	FReply Reply = FReply::Unhandled();
	if (IsEnabled() && FSlateApplication::Get().GetNavigationActionFromKey(InKeyEvent) == EUINavigationAction::Accept)
	{
		Press();

		if (PressMethod == EButtonPressMethod::ButtonPress)
		{
			//execute our "OnClicked" delegate, and get the reply
			Reply = Click();

			//You should ALWAYS handle the OnClicked event.
			ensure(Reply.IsEventHandled() == true);
		}
		else
		{
			Reply = FReply::Handled();
		}
	}
	else
	{
		Reply = SAdvPanel::OnKeyDown(MyGeometry, InKeyEvent);
	}

	//return the constructed reply
	return Reply;
}

FReply SAdvButtonBase::OnKeyUp(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	FReply Reply = FReply::Unhandled();

	if (IsEnabled() && FSlateApplication::Get().GetNavigationActionFromKey(InKeyEvent) == EUINavigationAction::Accept)
	{
		const bool bWasPressed = bIsPressed;

		Release();

		if (PressMethod == EButtonPressMethod::ButtonRelease || (PressMethod == EButtonPressMethod::DownAndUp && bWasPressed))
		{
			//execute our "OnClicked" delegate, and get the reply
			Reply = Click();

			//You should ALWAYS handle the OnClicked event.
			ensure(Reply.IsEventHandled() == true);
		}
		else
		{
			Reply = FReply::Handled();
		}
	}

	//return the constructed reply
	return Reply;
}

FReply SAdvButtonBase::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	FReply Reply = FReply::Unhandled();
	if (IsEnabled() && (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton || MouseEvent.IsTouchEvent()))
	{
		Press();
		PressedScreenSpacePosition = MouseEvent.GetScreenSpacePosition();

		EButtonClickMethod::Type InputClickMethod = GetClickMethodFromInputType(MouseEvent);

		if (InputClickMethod == EButtonClickMethod::MouseDown)
		{
			//get the reply from the execute function
			Reply = Click();

			//You should ALWAYS handle the OnClicked event.
			ensure(Reply.IsEventHandled() == true);
		}
		else if (InputClickMethod == EButtonClickMethod::PreciseClick)
		{
			// do not capture the pointer for precise taps or clicks
			// 
			Reply = FReply::Handled();
		}
		else
		{
			//we need to capture the mouse for MouseUp events
			Reply = FReply::Handled().CaptureMouse(AsShared());
		}
	}

	Invalidate(EInvalidateWidget::Layout);

	//return the constructed reply
	return Reply;
}

FReply SAdvButtonBase::OnMouseButtonDoubleClick(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent)
{
	return OnMouseButtonDown(InMyGeometry, InMouseEvent);
}

FReply SAdvButtonBase::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	FReply                         Reply                     = FReply::Unhandled();
	const EButtonClickMethod::Type InputClickMethod          = GetClickMethodFromInputType(MouseEvent);
	const bool                     bMustBePressed            = InputClickMethod == EButtonClickMethod::DownAndUp || InputClickMethod == EButtonClickMethod::PreciseClick;
	const bool                     bMeetsPressedRequirements = (!bMustBePressed || (bIsPressed && bMustBePressed));

	if (bMeetsPressedRequirements && ((MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton || MouseEvent.IsTouchEvent())))
	{
		Release();

		if (IsEnabled())
		{
			if (InputClickMethod == EButtonClickMethod::MouseDown)
			{
				// NOTE: If we're configured to click on mouse-down/precise-tap, then we never capture the mouse thus
				//       may never receive an OnMouseButtonUp() call.  We make sure that our bIsPressed
				//       state is reset by overriding OnMouseLeave().
			}
			else
			{
				bool bEventOverButton = IsHovered();

				if (!bEventOverButton && MouseEvent.IsTouchEvent())
				{
					bEventOverButton = MyGeometry.IsUnderLocation(MouseEvent.GetScreenSpacePosition());
				}

				if (bEventOverButton)
				{
					// If we asked for a precise tap, all we need is for the user to have not moved their pointer very far.
					const bool bTriggerForTouchEvent = InputClickMethod == EButtonClickMethod::PreciseClick;

					// If we were asked to allow the button to be clicked on mouse up, regardless of whether the user
					// pressed the button down first, then we'll allow the click to proceed without an active capture
					const bool bTriggerForMouseEvent = (InputClickMethod == EButtonClickMethod::MouseUp || HasMouseCapture());

					if ((bTriggerForTouchEvent || bTriggerForMouseEvent))
					{
						Reply = Click();
					}
				}
			}
		}

		//If the user of the button didn't handle this click, then the button's
		//default behavior handles it.
		if (Reply.IsEventHandled() == false)
		{
			Reply = FReply::Handled();
		}
	}

	//If the user hasn't requested a new mouse captor and the button still has mouse capture,
	//then the default behavior of the button is to release mouse capture.
	if (Reply.GetMouseCaptor().IsValid() == false && HasMouseCapture())
	{
		Reply.ReleaseMouseCapture();
	}

	Invalidate(EInvalidateWidget::Layout);

	return Reply;
}

FReply SAdvButtonBase::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (IsPressed() && IsPreciseTapOrClick(MouseEvent) && FSlateApplication::Get().HasTraveledFarEnoughToTriggerDrag(MouseEvent, PressedScreenSpacePosition))
	{
		Release();
	}

	return FReply::Unhandled();
}

void SAdvButtonBase::OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	SAdvPanel::OnMouseEnter(MyGeometry, MouseEvent);

	Hover();

	Invalidate(EInvalidateWidget::Layout);
}

void SAdvButtonBase::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	const bool bWasHovered = IsHovered();

	// Call parent implementation
	SAdvPanel::OnMouseLeave(MouseEvent);

	// If we're setup to click on mouse-down, then we never capture the mouse and may not receive a
	// mouse up event, so we need to make sure our pressed state is reset properly here
	if (ClickMethod == EButtonClickMethod::MouseDown || IsPreciseTapOrClick(MouseEvent))
	{
		Release();
	}

	if (bWasHovered)
	{
		Unhover();
	}

	Invalidate(EInvalidateWidget::Layout);
}

void SAdvButtonBase::OnMouseCaptureLost(const FCaptureLostEvent& CaptureLostEvent)
{
	Release();
}

FReply SAdvButtonBase::Click()
{
	OnClick();
	if (OnClicked.IsBound())
	{
		FReply Reply = OnClicked.Execute();
#if WITH_ACCESSIBILITY
		// @TODOAccessibility: This should pass the Id of the user that clicked the button but we don't want to change the regular Slate API just yet
		FSlateApplicationBase::Get().GetAccessibleMessageHandler()->OnWidgetEventRaised(FSlateAccessibleMessageHandler::FSlateWidgetAccessibleEventArgs(AsShared(), EAccessibleEvent::Activate));
#endif
		return Reply;
	}
	return FReply::Handled();
}

void SAdvButtonBase::Press()
{
	if (!bIsPressed)
	{
		bIsPressed = true;

		OnPress();
		OnPressed.ExecuteIfBound();
	}
}

void SAdvButtonBase::Release()
{
	if (bIsPressed)
	{
		bIsPressed = false;

		OnRelease();
		OnReleased.ExecuteIfBound();
	}
}

void SAdvButtonBase::Hover()
{
	OnHover();
	OnHovered.ExecuteIfBound();
}

void SAdvButtonBase::Unhover()
{
	OnUnhover();
	OnUnhovered.ExecuteIfBound();
}

void SAdvButtonBase::OnClick()
{
}

void SAdvButtonBase::OnPress()
{
}

void SAdvButtonBase::OnRelease()
{
}

void SAdvButtonBase::OnHover()
{
}

void SAdvButtonBase::OnUnhover()
{
}

bool SAdvButtonBase::IsInteractable() const
{
	return IsEnabled();
}

bool SAdvButtonBase::ComputeVolatility() const
{
	return SAdvPanel::ComputeVolatility();
}

TEnumAsByte<EButtonClickMethod::Type> SAdvButtonBase::GetClickMethodFromInputType(const FPointerEvent& MouseEvent) const
{
	if (MouseEvent.IsTouchEvent())
	{
		switch (TouchMethod)
		{
		case EButtonTouchMethod::Down:
			return EButtonClickMethod::MouseDown;
		case EButtonTouchMethod::DownAndUp:
			return EButtonClickMethod::DownAndUp;
		case EButtonTouchMethod::PreciseTap:
			return EButtonClickMethod::PreciseClick;
		default:
			return EButtonClickMethod::DownAndUp;
		}
	}

	return ClickMethod;
}

bool SAdvButtonBase::IsPreciseTapOrClick(const FPointerEvent& MouseEvent) const
{
	return GetClickMethodFromInputType(MouseEvent) == EButtonClickMethod::PreciseClick;
}

FVector2D SAdvButtonBase::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
	return SAdvPanel::ComputeDesiredSize(LayoutScaleMultiplier);
}

void SAdvButtonBase::SetOnClicked(FOnClicked InOnClicked)
{
	OnClicked = InOnClicked;
}

void SAdvButtonBase::SetOnPressed(FSimpleDelegate InOnPressed)
{
	OnPressed = InOnPressed;
}

void SAdvButtonBase::SetOnReleased(FSimpleDelegate InOnReleased)
{
	OnReleased = InOnReleased;
}

void SAdvButtonBase::SetOnHovered(FSimpleDelegate InOnHovered)
{
	OnHovered = InOnHovered;
}

void SAdvButtonBase::SetOnUnhovered(FSimpleDelegate InOnUnhovered)
{
	OnUnhovered = InOnUnhovered;
}

void SAdvButtonBase::SetClickMethod(EButtonClickMethod::Type InClickMethod)
{
	ClickMethod = InClickMethod;
}

void SAdvButtonBase::SetTouchMethod(EButtonTouchMethod::Type InTouchMethod)
{
	TouchMethod = InTouchMethod;
}

void SAdvButtonBase::SetPressMethod(EButtonPressMethod::Type InPressMethod)
{
	PressMethod = InPressMethod;
}

#if WITH_ACCESSIBILITY
TSharedRef<FSlateAccessibleWidget> SAdvButtonBase::CreateAccessibleWidget()
{
	return MakeShareable<FSlateAccessibleWidget>(new FSlateAccessibleButton(SharedThis(this)));
}
#endif
