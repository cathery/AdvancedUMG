// Copyright (c) 2022 cathery

#include "UMG/AdvButtonBase.h"

#include "Widgets/DeclarativeSyntaxSupport.h"

#include "Slate/SAdvButtonBase.h"

UAdvBaseButton::UAdvBaseButton()
	: ClickMethod(EButtonClickMethod::DownAndUp)
	, TouchMethod(EButtonTouchMethod::DownAndUp)
	, PressMethod(EButtonPressMethod::DownAndUp)
	, IsFocusable(false)
{
	bIsVariable = true;
	Visibility  = ESlateVisibility::Visible;

#if WITH_EDITORONLY_DATA
	AccessibleBehavior       = ESlateAccessibleBehavior::Summary;
	bCanChildrenBeAccessible = false;
#endif
}

bool UAdvBaseButton::IsPressed() const
{
	if (GetSlateAdvButton())
	{
		return GetSlateAdvButton()->IsPressed();
	}

	return false;
}

void UAdvBaseButton::SetClickMethod(EButtonClickMethod::Type InClickMethod)
{
	ClickMethod = InClickMethod;
	if (GetSlateAdvButton())
	{
		GetSlateAdvButton()->SetClickMethod(ClickMethod);
	}
}

void UAdvBaseButton::SetTouchMethod(EButtonTouchMethod::Type InTouchMethod)
{
	TouchMethod = InTouchMethod;
	if (GetSlateAdvButton())
	{
		GetSlateAdvButton()->SetTouchMethod(TouchMethod);
	}
}

void UAdvBaseButton::SetPressMethod(EButtonPressMethod::Type InPressMethod)
{
	PressMethod = InPressMethod;

	if (GetSlateAdvButton())
	{
		GetSlateAdvButton()->SetPressMethod(PressMethod);
	}
}

void UAdvBaseButton::SynchronizeProperties()
{
	Super::SynchronizeProperties();
}

FReply UAdvBaseButton::SlateHandleClicked()
{
	OnClicked.Broadcast();
	return FReply::Handled();
}

void UAdvBaseButton::SlateHandlePressed()
{
	OnPressed.Broadcast();
}

void UAdvBaseButton::SlateHandleReleased()
{
	OnReleased.Broadcast();
}

void UAdvBaseButton::SlateHandleHovered()
{
	OnHovered.Broadcast();
}

void UAdvBaseButton::SlateHandleUnhovered()
{
	OnUnhovered.Broadcast();
}

TSharedPtr<SAdvPanel> UAdvBaseButton::GetSlateAdvPanel() const
{
	return GetSlateAdvButton();
}

void UAdvBaseButton::MakeSlateWidget()
{
	GetSlateAdvButton() = SNew(SAdvButtonBase)
		.ClickMethod(ClickMethod)
		.TouchMethod(TouchMethod)
		.PressMethod(PressMethod)
		.IsFocusable(IsFocusable)
		.OnClicked_UObject(this, &ThisClass::SlateHandleClicked)
		.OnPressed_UObject(this, &ThisClass::SlateHandlePressed)
		.OnReleased_UObject(this, &ThisClass::SlateHandleReleased)
		.OnHovered_UObject(this, &ThisClass::SlateHandleHovered)
		.OnUnhovered_UObject(this, &ThisClass::SlateHandleUnhovered);
}

TSharedPtr<SAdvButtonBase> UAdvBaseButton::GetSlateAdvButton() const
{
	return nullptr;
}
